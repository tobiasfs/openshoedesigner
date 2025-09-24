///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLMaterialX.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 20.10.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////
#include "OpenGLMaterialX.h"

#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>

#ifdef USE_LIBZIP
#include <zip.h>
#endif
#ifdef USE_LIBPNG
#include <png.h>
#endif
#ifdef USE_LIBJPEG
#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <jpeglib.h>
#include <jerror.h>
#endif

using std::vector;
using std::map;
using std::string;

static const std::regex regexFileSearch("\\.mtlx$",
		std::regex::icase | std::regex::optimize);

// MATX files have no node-text. Everything is stored as an attribute.
static const std::regex regexXmlTokenizer(
		"\\s*(<[!]?--(?:.|[\r\n])*-->|<!.*?>|<\\?(?:.|[\r\n])*?\\?>|</|<|/>|>|\"[^\"<]*\"|'[^'<]*'|=|[_:a-z][_:a-z-\\.0-9]*)",
		std::regex::icase | std::regex::optimize);

static const auto iterEnd = std::sregex_iterator();

#define NextToken() xml_it++; if(xml_it == iterEnd) return; token = (*xml_it)[1]

void OpenGLMaterialX::Delete() {
	OpenGLShader::Delete();
	for (auto &image : images) {
		const GLuint id = image.second.textureID;
		if (id >= 1)
			glDeleteTextures(1, &id);
		image.second.textureID = 0;
	}
}

void OpenGLMaterialX::Load(const std::filesystem::path &filename_) {
#ifdef USE_LIBZIP
	int err = 0;
	zip_t *zip_file_handle = zip_open(filename_.c_str(), ZIP_RDONLY, &err);

	if (!zip_file_handle) {
		zip_error_t ziperror;
		zip_error_init_with_code(&ziperror, err);
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << " Error opening zipfile:";
		out << ' ' << zip_error_strerror(&ziperror);
		throw std::runtime_error(out.str());

	}
	zip_int64_t nentries = zip_get_num_entries(zip_file_handle, 0);
	zip_int64_t idx_mtlx = -1;
	for (zip_int64_t idx = 0; idx < nentries; idx++) {
		const char *temp = zip_get_name(zip_file_handle, idx, ZIP_FL_ENC_GUESS);
		string name(temp);
		if (std::regex_search(name, regexFileSearch)) {
			idx_mtlx = idx;
			break;
		}
	}
	if (idx_mtlx < 0) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << " The zip-file:" << filename_.string();
		out << " does not contain a .mtlx file (MaterialX file).";
		throw std::runtime_error(out.str());
	}

	zip_file_t *mtlxfile = zip_fopen_index(zip_file_handle, idx_mtlx, 0);
	if (!mtlxfile) {
		zip_error_t ziperror;
		zip_error_init_with_code(&ziperror, err);
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << " Error reading from the zipped file #" << idx_mtlx;
		out << ' ' << zip_error_strerror(&ziperror);
		throw std::runtime_error(out.str());
	}

	// Process XML in matx file

	string xml;
	{
		zip_int64_t bytes_read = 0;
		string buffer(4096, '\0');
		while ((bytes_read = zip_fread(mtlxfile, &buffer[0], 4096))) {
			xml.append(buffer, 0, bytes_read);
		}
		xml.append(buffer, 0, bytes_read);
		zip_fclose(mtlxfile);
	}

	std::sregex_iterator xmlTokens(xml.begin(), xml.end(), regexXmlTokenizer);
	ParseXML(xmlTokens);

	// Filter image nodes into images

	for (const auto &node : nodes) {
		if (node.type == "nodegraph") {
			for (auto &nodeg : node.nodes) {
				if ((nodeg.second.type == "image"
						|| nodeg.second.type == "tiledimage")
						&& nodeg.second.inputs.find("file")
								!= nodeg.second.inputs.end()) {
					std::filesystem::path imagefile(
							nodeg.second.inputs.at("file").text);
					images.emplace(std::make_pair(imagefile, Image(imagefile)));
				}
			}
		}
		if ((node.type == "image" || node.type == "tiledimage")
				&& node.inputs.find("file") != node.inputs.end()) {
			std::filesystem::path imagefile(node.inputs.at("file").text);
			images.emplace(std::make_pair(imagefile, Image(imagefile)));
		}
	}

	// Load images from .zip

	GLuint textureUnit = 0;
	for (auto &image : images) {
		zip_file_t *imagefile = zip_fopen(zip_file_handle,
				image.second.filename.c_str(), 0);
		if (!imagefile) {
			zip_error_t ziperror;
			zip_error_init_with_code(&ziperror, err);
			std::ostringstream out;
			out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
			out << " Error reading from the zipped file \"" << imagefile;
			out << "\" " << zip_error_strerror(&ziperror);
			throw std::runtime_error(out.str());
		}
		string imagedata;
		zip_int64_t bytes_read = 0;
		string buffer(4096, '\0');
		while ((bytes_read = zip_fread(imagefile, &buffer[0], 4096))) {
			imagedata.append(buffer, 0, bytes_read);
		}
		imagedata.append(buffer, 0, bytes_read);
		image.second.LoadFromString(imagedata);
		image.second.textureUnit = textureUnit++;
		zip_fclose(imagefile);
	}
	zip_close(zip_file_handle);
#else
	throw std::runtime_error(
			"OpenGLMaterialX::Load - Cannot be used because USE_LIBZIP was not set.");
#endif
}

void OpenGLMaterialX::ParseXML(std::sregex_iterator &xml_it) {
	do {
		string token = (*xml_it)[1];
		if (token == "<") {
			Node dummyNode;
			ParseXMLNode(xml_it, dummyNode);
		} else {
			// Skip special xml-nodes.
			xml_it++;
		}
	} while (xml_it != iterEnd);
}

void OpenGLMaterialX::ParseXMLNode(std::sregex_iterator &xml_it, Node &node) {
	string token;
	NextToken();
	string node_name = token;
	NextToken();

	if (token != "/>" && token != ">") {
		node.type = node_name;
		// Found Attributes
		if (node_name == "input") {
			Port inp;
			ParsePortAttributes(xml_it, inp);
			node.inputs[inp.name] = inp;
		} else if (node_name == "output") {
			Port outp;
			ParsePortAttributes(xml_it, outp);
			node.outputs[outp.name] = outp;
		} else {
			ParseNodeAttributes(xml_it, node);
		}
		token = (*xml_it)[1];
	}
	if (token == "/>") {
		NextToken();
		return;
	}
	if (token == ">") {
		NextToken();
		while (token == "<") {
			Node node_;
			ParseXMLNode(xml_it, node_);
			token = (*xml_it)[1];
			if (node.type == "materialx") {
				nodes.push_back(node_);
			} else if (node.type == "nodegraph") {
				if (node_.type == "output") {
					node.outputs.insert(node_.outputs.begin(),
							node_.outputs.end());
				} else {
					node.nodes[node_.name] = node_;
				}
			} else {
				node.inputs.insert(node_.inputs.begin(), node_.inputs.end());
			}
		}
		if (token != "</") {
			std::cerr << __FILE__ << ":" << __FUNCTION__ << ":";
			std::cerr << " Missing node closing for <" << node_name << " ...\n";
			return;
		}
		NextToken();
		if (token != node_name) {
			std::cerr << __FILE__ << ":" << __FUNCTION__ << ":";
			std::cerr << " Closing for \"" << token << "\" found.";
			std::cerr << " Expected \"" << node_name << "\".\n";
		}
		NextToken();
		if (token != ">") {
			std::cerr << __FILE__ << ":" << __FUNCTION__ << ":";
			std::cerr << " Missing closing '>'.\n";
			return;
		}
		NextToken();
		return;
	}
}

void OpenGLMaterialX::ParseNodeAttributes(std::sregex_iterator &xml_it,
		Node &node) {
	string token = (*xml_it)[1];
	do {
		string id = token;
		NextToken();
		string op = token;
		if (op != "=") {
			std::cerr << __FILE__ << ":" << __FUNCTION__ << ":";
			std::cerr << " Unexpected operator '" << token << "'.";
			std::cerr << " Expected '='.\n";
			return;
		}
		NextToken();
		string value = token;
		if (value.front() == '"')
			value = value.substr(1);
		if (value.back() == '"')
			value.pop_back();
		NextToken();

		if (id == "name")
			node.name = value;
		if (id == "type")
			node.datatype = value;

	} while (token != ">" && token != "/>");
}

void OpenGLMaterialX::ParsePortAttributes(std::sregex_iterator &xml_it,
		Port &port) {
	string token = (*xml_it)[1];
	do {
		string id = token;
		NextToken();
		string op = token;
		if (op != "=") {
			std::cerr << __FILE__ << ":" << __FUNCTION__ << ":";
			std::cerr << " Unexpected operator '" << token << "'.";
			std::cerr << " Expected '='.\n";
			return;
		}
		NextToken();
		string value = token;
		if (value.front() == '"')
			value = value.substr(1);
		if (value.back() == '"')
			value.pop_back();
		NextToken();

		if (id == "name")
			port.name = value;
		if (id == "type")
			port.type = value;
		if (id == "nodename")
			port.ref_name = value;
		if (id == "value")
			port.text = value;

	} while (token != ">" && token != "/>");
}

OpenGLMaterialX::Image::Image(const std::filesystem::path &filename_) :
		filename(filename_) {
}

OpenGLMaterialX::Image::~Image() {
	if (textureID >= 1)
		glDeleteTextures(1, &textureID);
}

void OpenGLMaterialX::Image::LoadFromString(std::string &imagedata) {

	if (imagedata.substr(0, 3) == "\xff\xd8\xff") {
#ifdef USE_LIBJPEG

		struct jpeg_error_mgr jerr;
		struct jpeg_decompress_struct cinfo;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_mem_src(&cinfo, reinterpret_cast<unsigned char*>(imagedata.data()),
				imagedata.size());
		(void) jpeg_read_header(&cinfo, TRUE);
		(void) jpeg_start_decompress(&cinfo);

		this->w = cinfo.output_width;
		this->h = cinfo.output_height;
		this->bitsPerComponent = cinfo.data_precision;
		this->componentsPerPixel = cinfo.output_components;

		const size_t bytesNeeded = (this->w * this->h * this->bitsPerComponent
				* this->componentsPerPixel) / 8;
		this->data.resize(bytesNeeded);
		if (this->data.size() == 0)
			throw std::runtime_error(
					" : Could not allocate memory for jpeg-image.");

		const int rowStride = cinfo.output_width * cinfo.output_components
				* (cinfo.data_precision / 8);

		if (cinfo.data_precision != 12) {
			while (cinfo.output_scanline < cinfo.output_height) {
				unsigned char *rowPointer = &this->data[rowStride
						* cinfo.output_scanline];
				(void) jpeg_read_scanlines(&cinfo, &rowPointer, 1);
			}
		} else {
			throw std::runtime_error(
					" : 12 bit JPEGs are not implemented/tested.");
//				int little_endian = 1;
			/* Make a one-row-high sample array that will go away when done with image */
//			if (cinfo.data_precision == 12)
//				buffer12 = (J12SAMPARRAY)(*cinfo.mem->alloc_sarray)(
//						(j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
//			else
//			buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo,
//			JPOOL_IMAGE, row_stride, 1);
			/* Step 6: while (scan lines remain to be read) */
			/*           jpeg_read_scanlines(...); */
			/* Here we use the library's state variable cinfo->output_scanline as the
			 * loop counter, so that we don't have to keep track ourselves.
			 */
//			if (cinfo->data_precision == 12) {
//				while (cinfo->output_scanline < cinfo->output_height) {
//					/* jpeg12_read_scanlines expects an array of pointers to scanlines.
//					 * Here the array is only one element long, but you could ask for
//					 * more than one scanline at a time if that's more convenient.
//					 */
//					(void) jpeg12_read_scanlines(cinfo, buffer12, 1);
//					if (*(char*) &little_endian == 1) {
//						/* Swap MSB and LSB in each sample */
//						for (col = 0; col < row_stride; col++)
//							buffer12[0][col] = ((buffer12[0][col] & 0xFF) << 8)
//									| ((buffer12[0][col] >> 8) & 0xFF);
//					}
//					fwrite(buffer12[0], 1, row_stride * sizeof(J12SAMPLE),
//							outfile);
//				}
//			}
		}
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
#else
		throw std::runtime_error(
				"MaterialX - USE_LIBJPEG is not set. JPEG images cannot be used as textures.");
#endif

	} else if (imagedata.substr(0, 4) == "\x89PNG") {
#ifdef USE_LIBPNG
		png_image pngimage;

		std::memset(&pngimage, 0, (sizeof pngimage));
		pngimage.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_memory(&pngimage, imagedata.data(),
				imagedata.size()) == 0)
			throw std::runtime_error("Could not process PNG data.");

		if (pngimage.format & PNG_FORMAT_FLAG_LINEAR)
			this->bitsPerComponent = 16;
		else
			this->bitsPerComponent = 8;
		if (pngimage.format & PNG_FORMAT_FLAG_ALPHA) {
			pngimage.format = PNG_FORMAT_RGBA;
			this->componentsPerPixel = 4;
		} else {
			pngimage.format = PNG_FORMAT_RGB;
			this->componentsPerPixel = 3;
		}

		this->data.resize(PNG_IMAGE_SIZE(pngimage));

		if (this->data.size() == 0)
			throw std::runtime_error(
					" : Could not allocate memory for pngimage.");
		if (png_image_finish_read(&pngimage, nullptr, this->data.data(), 0,
				nullptr) == 0)
			throw std::runtime_error(
					" : Could not finish reading the pngimage.");
		this->w = pngimage.width;
		this->h = pngimage.height;

//		size_t N = this->w * this->h;
//			glGenTextures(1,&x);
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->w,
//					this->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
//					this->data.data());
//			const int bpc = (this->format == GL_RGB) ? 3 : 4;
//			this->rgba.assign(buffer, buffer + N * bpc);
//			free(buffer);
		png_image_free(&pngimage);

#else
		throw std::runtime_error(
				"MaterialX - USE_LIBPNG is not set. PNG images cannot be used as textures.");
#endif

	} else {
		std::cout << "Unknown file format.\n";
	}
	FlipY();
}

void OpenGLMaterialX::Image::FlipY() {
	unsigned int bytesPerLine = w * componentsPerPixel;
	for (unsigned int i = 0; i < (h / 2); i++) {
		uint8_t *p0 = data.data() + i * bytesPerLine;
		uint8_t *p1 = data.data() + (h - i - 1) * bytesPerLine;
		for (unsigned int j = 0; j < bytesPerLine; j++) {
			const uint8_t temp = *p0;
			*p0 = *p1;
			*p1 = temp;
			p0++;
			p1++;
		}
	}
}

void OpenGLMaterialX::Image::BindToTextureUnit() const {
	if (textureID < 1)
		return;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint OpenGLMaterialX::Image::LoadIntoGPU() {
	if (textureID >= 1)
		return textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum type = 0;
	if (bitsPerComponent == 8)
		type = GL_UNSIGNED_BYTE;
	else if (bitsPerComponent == 16)
		type = GL_UNSIGNED_SHORT;
	else if (bitsPerComponent == 32)
		type = GL_UNSIGNED_INT;
	else
		throw std::runtime_error(
				"OpenGLMaterialX::Image::PrepareTexture - Unhandeled number of bits per component.");

	GLenum format = GL_RED;
	if (componentsPerPixel == 1)
		format = GL_RED;
	else if (componentsPerPixel == 2)
		format = GL_RG;
	else if (componentsPerPixel == 3)
		format = GL_RGB;
	else if (componentsPerPixel == 4)
		format = GL_RGBA;
	else
		throw std::runtime_error(
				"OpenGLMaterialX::Image::PrepareTexture - Unhandeled number of components per pixel.");

	type = GL_UNSIGNED_BYTE; //TODO Check type and internal format.

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureUnit;
}

OpenGLMaterialX::Variable::Variable(Direction dir_, std::string type_,
		std::string name_, int location_) :
		dir(dir_), type(type_), name(name_), location(location_) {
}

bool OpenGLMaterialX::Variable::operator <(const Variable &rhs) const {
	if (dir < rhs.dir)
		return true;
	if (dir > rhs.dir)
		return false;
	return name.compare(rhs.name) < 0;
}

std::string OpenGLMaterialX::Variable::operator ()() const {
	std::ostringstream out;
	if (dir == Direction::VAR) {
		out << "var_";
	} else if (dir == Direction::PARAM) {
		out << "param_";
	} else if (dir == Direction::IN) {
		out << "in_";
	} else if (dir == Direction::OUT) {
		out << "out_";
	} else if (dir == Direction::UNIFORM) {
		out << "uni_";
	} else {
		throw std::runtime_error(
				"OpenGLMaterialX::Variable::ToString - This Direction enum was not implemented.");
	}
	out << name;
	return out.str();
}

std::string OpenGLMaterialX::Variable::ToDeclaration() const {
	std::ostringstream out;
	if (location >= 0 && (dir == Direction::IN || dir == Direction::OUT))
		out << "layout (location = " << location << ") ";
	// Only for 4.20+ GLSL:
	//	if (location >= 0 && dir == Direction::UNIFORM)
	//		out << "layout (binding = " << location << ") ";

	if (dir == Direction::IN) {
		out << "in ";
	} else if (dir == Direction::OUT) {
		out << "out ";
	} else if (dir == Direction::UNIFORM) {
		out << "uniform ";
	} else if (dir == Direction::VAR) {
	} else {
		throw std::runtime_error(
				"OpenGLMaterialX::Variable::ToDeclaration - This Direction is not used in the Declaration.");
	}
	if (!type.empty())
		out << type << ' ';
	out << (*this)() << ';';
	return out.str();
}

std::string OpenGLMaterialX::Variable::ToDefinition() const {
	std::ostringstream out;
	if (dir != Direction::PARAM && dir != Direction::VAR)
		throw std::runtime_error(
				"OpenGLMaterialX::Variable::ToDefinition - This Direction is not used in the Definition.");
	if (!type.empty())
		out << type << ' ';
	out << (*this)();
	return out.str();
}

std::string OpenGLMaterialX::Port::ToValue(const std::string &param_,
		const std::string &requiredType) const {
	std::ostringstream out;
	const string type_ = OpenGLMaterialX::ToGlslType(type);
	if (!ref_name.empty()) {
		out << ref_name;
		out << "(" << param_ << ")";
	} else if (!text.empty()) {
		if (type_ == "mat3" || type_ == "mat4")
			out << "transpose(";

		if (type_ == "float" || type_ == "bool" || type_ == "int")
			out << text;
		else
			out << type_ << "(" << text << ")";

		if (type_ == "mat3" || type_ == "mat4")
			out << ")";
	} else {
		if (requiredType == "float" || requiredType == "bool"
				|| requiredType == "int")
			out << "0.0";
		else
			out << requiredType << "(0.0)";
	}

	if (!requiredType.empty() && requiredType != type_) {
		if (requiredType == "float" || requiredType == "bool"
				|| requiredType == "int")
			out << ".r";
		else if (requiredType == "vec2")
			out << ".rg";
		else if (requiredType == "vec3")
			out << ".rgb";
		else if (requiredType == "vec4")
			out << ".rgba";
	}
	return out.str();
}

OpenGLMaterialX::Variable OpenGLMaterialX::Port::ToDefinition(std::ostream &out,
		const std::string &param, const std::string &requiredType) const {

	string type_ = OpenGLMaterialX::ToGlslType(type);
	Variable var(Variable::PARAM, requiredType, name);
	if (requiredType.empty())
		var.type = type_;

	if (ref_name.empty() && text.empty() && requiredType.empty())
		return var;

	out << '\t' << var.ToDefinition() << " = ";
	out << ToValue(param, requiredType);
	out << ";\n";
	var.valid = true;
	return var;
}

OpenGLMaterialX::Variable OpenGLMaterialX::Node::InputToString(
		std::ostream &out, const std::string &name_, const std::string &type_,
		const std::string &param_, const std::string &default_) const {

	Variable var(Variable::PARAM, type_, name_);

	const auto &idx = inputs.find(name_);
	if (idx == inputs.end()) {
		if (default_.empty() || type_.empty())
			return var;
		out << '\t' << "" << type_ << " param_" << name_ << " = ";
		if (type_ == "float" || type_ == "bool" || type_ == "int")
			out << default_;
		else
			out << type_ << "(" << default_ << ")";
		out << ";\n";
		var.valid = true;
		return var;
	} else {
		Variable temp = idx->second.ToDefinition(out, param_, type_);
		return temp;
	}
}

bool OpenGLMaterialX::Node::HasInput(const std::string &inp) const {
	return inputs.find(inp) != inputs.end();
}

void OpenGLMaterialX::Node::GenerateCode(std::ostream &out,
		std::set<Variable> &globals,
		std::map<std::filesystem::path, Image> &images_) {
	//		out << "//   out = " << n.datatype << '\n';
	//		for (const auto &p : ) {
	//			out << "//   in:" << p.second.name << " = ";
	//			out << p.second.type << '\n';
	//		}

	Variable in_frag_pos(Variable::IN, "vec3", "frag_pos");
	Variable in_view_pos(Variable::IN, "vec3", "view_pos");
	Variable in_light_pos(Variable::IN, "vec3", "light_pos");

	Variable in_tex_pos(Variable::IN, "vec2", "tex_pos");

	Variable out_frag_color(Variable::OUT, "vec4", "frag_color");

	globals.insert(in_frag_pos);
	globals.insert(in_view_pos);
	globals.insert(in_light_pos);
	globals.insert(in_tex_pos);

	globals.insert(out_frag_color);

	Variable param(Variable::PARAM, "vec2", "tex_coords");

	if (type == "tiledimage") {
		string outtype = ToGlslType(datatype);
		out << outtype << " " << name;
		out << "(" << param.ToDefinition() << ")\n";
		out << "{\n";
		out << '\t' << "// " << type << "\n";

		if (HasInput("file")) {

			auto filename = std::filesystem::path(inputs.at("file").text);
			GLuint textureUnit = images_.at(filename).LoadIntoGPU();
			Variable sampler(Variable::UNIFORM, "sampler2D", name,
					images_.at(filename).textureUnit);
			globals.insert(sampler);
			images_.at(filename).name = name;
			out << '\t' << "// Texture in Textureunit " << textureUnit << "\n";

			auto uvtiling = InputToString(out, "uvtiling", "vec2", param(), "");
			auto uvoffset = InputToString(out, "uvoffset", "vec2", param(), "");

			Variable modified(Variable::VAR, param.type,
					param.name + "_modified");

			out << '\t' << modified.ToDefinition() << " = " << param();
			if (uvtiling.valid)
				out << " * " << uvtiling();
			if (uvoffset.valid)
				out << " + " << uvoffset();
			out << ";\n";

			out << '\t' << "return texture";
			out << "(" << sampler() << ", " << modified() << ")";

			if (outtype == "vec4") {
				out << ".rgba";
			} else if (outtype == "vec3") {
				out << ".rgb";
			} else if (outtype == "vec2") {
				out << ".rg";
			} else {
				out << ".r";
			}
		} else {
			out << '\t' << "return " << outtype << "(0)";
		}
		out << ";\n";
		out << "}\n";

	} else if (type == "normalmap") {
		string outtype = ToGlslType(datatype);
		out << outtype << " " << name;
		out << "(" << param.ToDefinition() << ")\n";
		out << "{\n";
		out << '\t' << "// " << type << "\n";

		auto var_in = InputToString(out, "in", "vec3", param(), "0.0");
		auto scale = InputToString(out, "scale", "float", param(), "");

		out << '\t' << "return normalize(" << var_in() << " * 2.0 - 1.0)";
		if (scale.valid)
			out << " * " << scale();
		out << ";\n";
		out << "}\n";

	} else if (type == "standard_surface") {
		out << "vec4 " << name;
		out << "(" << param.ToDefinition() << ")\n";
		out << "{\n";
		out << '\t' << "// " << type << "\n";

		Variable light_pos(Variable::IN, "vec3", "light_pos");
		globals.insert(light_pos);
		Variable view_pos(Variable::IN, "vec3", "view_pos");
		globals.insert(view_pos);

		Variable light_color(Variable::UNIFORM, "vec3", "light_color");
		globals.insert(light_color);
		Variable ambient_strength(Variable::UNIFORM, "float",
				"ambient_strength");
		globals.insert(ambient_strength);
		Variable diffuse_strength(Variable::UNIFORM, "float",
				"diffuse_strength");
		globals.insert(diffuse_strength);
		Variable specular_strength(Variable::UNIFORM, "float",
				"specular_strength");
		globals.insert(specular_strength);

		auto base_color = InputToString(out, "base_color", "vec3", param(),
				"0.8, 0.8, 0.8");
		auto opacity = InputToString(out, "opacity", "vec3", param(),
				"1.0, 1.0, 1.0");
		auto normalmap = InputToString(out, "normal", "vec3", param(),
				"0.0, 0.0, 1.0");
		auto specular_roughness = InputToString(out, "specular_roughness",
				"float", param(), "0.5");
		auto metalness = InputToString(out, "metalness", "float", param(), "");
		auto coat_roughness = InputToString(out, "coat_roughness", "float",
				param(), "0.5");
		auto coat_color = InputToString(out, "coat_color", "vec3", param(),
				"1.0, 1.0, 1.0");

		Variable normal(Variable::VAR, "vec3", "localNormal");

		Variable local_coord(Variable::UNIFORM, "bool", "local");
		globals.insert(local_coord);
		Variable in_normal(Variable::IN, "vec3", "normal");
		globals.insert(in_normal);

//		out
//				<< "\tfloat param_displacement = Bricks069_1K_JPG_Displacement(param_tex_coords);\n";

		out << "\n\t" << normal.ToDeclaration() << "\n";
		out << "\t" << "if(" << local_coord() << "){\n";
//		out << "\t\t" << normal() << " = vec3(0,0,1);\n";
		Variable heightScale(Variable::UNIFORM, "float", "height_scale");
		out << "\t\t" << normal() << " = normalize(vec3(vec2(" << normalmap()
				<< ".x, " << normalmap() << ".y) * 1.0" << ", " << normalmap()
				<< ".z));\n";
//		out << "\t" << normal()
//				<< " = Sponge001_1K_JPG_NormalGL(param_tex_coords);\n";
//		out << "\t" << "return vec4(" << param() << ".x, " << param()
//				<< ".y, 0, 1);\n";
//		out << "\t" << "return vec4(" << normalmap() << "*0.5+0.5, 1);\n";
		out << "\t" << "}else{\n";
		out << "\t\t" << normal() << " = " << in_normal() << ";\n";
//		out << "\t\treturn vec4(vec3(param_displacement), 1);\n";
		out << "\t" << "}\n";

		out << "\n\t" << "vec3 light_dir = normalize(" << light_pos() << " - "
				<< in_frag_pos() << "); \n";
		out << '\t' << "vec3 view_dir = normalize(" << view_pos() << " - "
				<< in_frag_pos() << ");\n";
		out << '\t' << "vec3 halfway_dir = normalize(light_dir + view_dir);\n";
		out << '\t' << "vec3 reflect_dir = reflect(-light_dir, " << normal()
				<< ");  \n";

		out << "\n\t" << "vec3 ambient = " << light_color() << " * "
				<< ambient_strength() << ";\n";

		out << "\n\t" << "float diff = max(dot(" << normal()
				<< ", light_dir), 0.0);\n";
		out << '\t' << "vec3 diffuse = diff * " << light_color() << " * "
				<< diffuse_strength() << ";\n";
		if (metalness.valid)
			out << "\n\t" << "float specular_strength = " << metalness()
					<< ";\n";
		Variable blinn(Variable::UNIFORM, "bool", "blinn");
		globals.insert(blinn);

		Variable shininess(Variable::UNIFORM, "float", "shininess");
		globals.insert(shininess);

		out << "\t" << "float spec = 0.0;\n";
		out << "\t" << "const float pi_ = " << M_PI << ";\n";

		out << "\t" << "if(" << blinn() << "){\n";
		out << "\t\t" << "float ec = ( 8.0 + " << shininess()
				<< " ) / ( 8.0 * pi_ );\n";
		out << "\t\t" << "spec = ec * pow(max(dot(" << normal()
				<< ", halfway_dir), 0.0), "
//				<< " 16.1) * " << shininess() << ";\n";
				<< shininess() << ");\n";
		out << "\t" << "}else{\n";
		out << "\t\t" << "float ec = ( 2.0 + " << shininess()
				<< " ) / ( 2.0 * pi_ );\n";
		out << "\t\t"
				<< "spec = ec * pow(max(dot(view_dir, reflect_dir), 0.0), "
				<< shininess() << ");\n";
		out << "\t" << "}\n";
//		out << "\t" << "spec = 0.0;\n";
		out << '\t' << "vec3 specular = " << specular_roughness()
				<< " * spec * " << light_color() << " * " << specular_strength()
				<< ";\n";

		out << "\n\t" << "vec3 result = (ambient + diffuse + specular) * "
				<< base_color() << ";\n";
		out << '\t' << "return vec4(result, " << opacity() << ".x);\n";
		out << "}\n";

	} else if (type == "displacement") {
		Variable heightScale(Variable::UNIFORM, "float", "height_scale");
		globals.insert(heightScale);

		Variable view_dir(Variable::PARAM, "vec3", "view_dir");
		out << "vec2 " << name;
		out << "(" << param.ToDefinition() << ", " << view_dir.ToDefinition()
				<< ")\n";
		out << "{\n";
		out << '\t' << "// " << type << "\n";

		Variable curCorr(Variable::VAR, "vec2", "current_coordinates");

		out << "\t" << curCorr.ToDefinition() << " = " << param() << ";\n";

		const auto &dmv = inputs.find("displacement");
		if (dmv == inputs.end()) {
			out << "\t" << "return " << param() << ";\n";
			return;
		}

		Variable displacement(Variable::VAR, "float", "displacement");

		out << "\t" << displacement.ToDefinition() << " = (1.0 - "
				<< dmv->second.ToValue(param()) << ");\n";

		Variable numLayers(Variable::VAR, "float", "numLayers");
		Variable layerDepth(Variable::VAR, "float", "layerDepth");
		Variable currLayerDepth(Variable::VAR, "float", "currentLayerDepth");

		out << "\t" << "const float minLayers = 8;\n";
		out << "\t" << "const float maxLayers = 32;\n";

		out << "\t" << numLayers.ToDefinition()
				<< " = mix(maxLayers, minLayers, max(dot(vec3(0,0,1), "
				<< view_dir() << "), 0.0));\n";
		out << "\t" << layerDepth.ToDefinition() << " = 1.0 / " << numLayers()
				<< ";\n";
		out << "\t" << currLayerDepth.ToDefinition() << " = 0.0;\n";

		auto scale = InputToString(out, "scale", "float", param(), "1.0");

		out << '\t' << "vec2 P = " << view_dir() << ".xy";
//		out << " / " << view_dir() << ".z";
		if (scale.valid)
			out << " * " << scale();
		out << " * " << heightScale() << ";\n";

		Variable delta(Variable::VAR, "vec2", "deltaTexCoords");
		out << "\t" << delta.ToDefinition() << " = P / " << numLayers()
				<< ";\n";

		out << "\t" << "while(" << currLayerDepth() << " < " << displacement()
				<< ")\n";
		out << "\t" << "{\n";
		out << "\t\t" << curCorr() << " -= " << delta() << ";\n";
		out << "\t\t" << displacement() << " = (1.0 - "
				<< dmv->second.ToValue(curCorr()) << ");\n";
		out << "\t\t" << currLayerDepth() << " += " << layerDepth() << ";\n";
		out << "\t" << "}\n";

		out << '\t' << "return " << curCorr() << ";\n";
		out << "}\n";

	} else if (type == "surfacematerial") {
		out << "void main() //" << name << "\n";
		out << "{\n";
		out << '\t' << "// " << type << "\n";

		const Port &ref = inputs.at("surfaceshader");
		Variable var_out(Variable::VAR, "vec4", "out");
		if (HasInput("displacementshader")) {
			Variable modified(Variable::VAR, in_tex_pos.type,
					in_tex_pos.name + "_modified");

			Variable view_dir(Variable::VAR, "vec3", "view_dir");

			const Port &refd = inputs.at("displacementshader");
			out << '\t' << view_dir.ToDefinition() << " = normalize("
					<< in_view_pos();
			out << " - " << in_frag_pos() << ");\n";
			Variable shift(Variable::VAR, "vec2", "shift");
			Variable displacementstrength(Variable::UNIFORM, "float",
					"diplacement_strength");
			globals.insert(displacementstrength);
			out << '\t' << shift.ToDefinition();
			out << " = " << refd.ref_name;
			out << "(" << in_tex_pos() << ", " << view_dir() << ")";
			out << " * " << displacementstrength() << ";\n";

			out << '\t' << modified.type << " " << modified();
			out << " = " << in_tex_pos() << " * (1 - " << displacementstrength()
					<< ") + " << shift() << ";\n";
//			out << "\t" << "if(" << modified() << ".x > 1.0 || " << modified()
//					<< ".y > 1.0 || " << modified() << ".x < 0.0 || "
//					<< modified() << ".y < 0.0 ) discard;\n";

			out << '\t' << var_out.ToDefinition() << " = " << ref.ref_name;
			out << "(" << modified() << ");\n";
		} else {
			out << '\t' << var_out.ToDefinition() << " = " << ref.ref_name;
			out << "(" << in_tex_pos() << ");\n";
		}
		out << '\t' << out_frag_color() << " = " << var_out() << ";\n";
		out << "}\n";

	} else {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "The node \"" << type << "\"";
		err << " has not code generation assigned (yet).";
		throw std::runtime_error(err.str());
	}

}

std::string OpenGLMaterialX::ToGlslType(const std::string &type) {

	if (type == "integer") {
		return "int";
	} else if (type == "boolean") {
		return "bool";
	} else if (type == "float") {
		return "float";
	} else if (type == "color3") {
		return "vec3";
	} else if (type == "color4") {
		return "vec4";

	} else if (type == "vector2") {
		return "vec2";
	} else if (type == "vector3") {
		return "vec3";
	} else if (type == "vector4") {
		return "vec4";

	} else if (type == "matrix33") {
		return "mat3";
	} else if (type == "matrix44") {
		return "mat4";

	} else {
#ifdef DEBUG
		return "?";
#else
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "The type \"" << type << "\" has not translation.";
		throw std::runtime_error(err.str());
#endif
	}
}

std::string OpenGLMaterialX::FieldString() {
	return "XYZTBNUV";
}

std::string OpenGLMaterialX::GenerateVertexShader() {

	std::ostringstream code;
	code << "#version 330 core \n\n";
	code << "\n";

	code << "layout (location = 0) in vec3 aPos;\n";
	code << "layout (location = 1) in vec3 aTangent;\n";
	code << "layout (location = 2) in vec3 aBitangent;\n";
	code << "layout (location = 3) in vec3 aNormal;\n";
	code << "layout (location = 4) in vec2 aTexCoords;\n";
	code << "\n";
	code << "out vec3 in_frag_pos;\n";
	code << "out vec3 in_light_pos;\n";
	code << "out vec2 in_tex_pos;\n";
	code << "out vec3 in_view_pos;\n";
	code << "\n";
	code << "out vec3 in_normal;\n";
	code << "\n";
	code << "uniform mat4 Projection;\n";
	code << "uniform mat4 View;\n";
	code << "uniform mat4 Model;\n";
	code << "uniform mat3 ModelNormal;\n";
	code << "\n";
	code << "uniform vec3 uni_light_pos;\n";
	code << "uniform vec3 uni_view_pos;\n";
	code << "uniform bool uni_local;\n";
	code << "\n";
	code << "void main()\n";
	code << "{\n";
	code << "gl_Position =  Projection * View * Model * vec4(aPos, 1.0);\n";
	code << "\n";
	code << "vec3 tangent = normalize(ModelNormal * aTangent);\n";
	code << "vec3 bitangent = normalize(ModelNormal * aBitangent);\n";
	code << "vec3 normal = normalize(ModelNormal * aNormal);\n";
	code << "\n";
	code << "in_normal = normal;\n";
	code << "\n";
	code << "if(uni_local){\n";
	code << "mat3 iTBN = transpose(mat3(tangent, bitangent, normal));\n";
	code << "\n";
	code << "in_view_pos = iTBN * uni_view_pos;\n";
	code << "in_light_pos = iTBN * uni_light_pos;\n";
	code << "in_frag_pos = iTBN * vec3(Model * vec4(aPos, 1.0));\n";
	code << "}else{\n";
	code << "in_view_pos = uni_view_pos;\n";
	code << "in_light_pos = uni_light_pos;\n";
	code << "in_frag_pos = vec3(Model * vec4(aPos, 1.0));\n";
	code << "}\n";
	code << "\n";
	code << "in_tex_pos = aTexCoords;\n";
	code << "}\n";

//		std::vector<Variable> temp(globals.begin(), globals.end());
//		std::sort(temp.begin(), temp.end());
//		for (const auto &g : temp) {
//			code << g.ToDeclaration() << '\n';
//		}
//		code << out.str();

	return code.str();

}

std::string OpenGLMaterialX::GenerateFragmentShader() {

	// List all possible nodes names
	vector<string> names(nodes.size());
	std::transform(nodes.begin(), nodes.end(), names.begin(),
			[](const auto &n) {
				return n.name;
			});

	// Filter for root node
	std::set<string> root(names.begin(), names.end());
	for (const Node &n : nodes) {
		for (const auto &p : n.inputs) {
			const auto &idx = root.find(p.second.ref_name);
			if (idx == root.end())
				continue;
			root.erase(idx);
		}
	}
	if (root.empty()) {
		throw std::runtime_error(" - The material has not root node.");
	}
	string root_name = *(root.begin());

	// Scan for all needed nodes based on the root node.
	size_t nLast = 0;
	std::vector<string> nodesList;
	nodesList.push_back(root_name);
	while (nLast < nodesList.size() || nodesList.empty()) {
		nLast = nodesList.size();
		for (const Node &n : nodes) {
			if (std::find(nodesList.begin(), nodesList.end(), n.name)
					== nodesList.end())
				continue;
			for (const auto &p : n.inputs) {
				if (std::find(nodesList.begin(), nodesList.end(),
						p.second.ref_name) != nodesList.end())
					continue;
				if (p.second.ref_name.empty())
					continue;
				nodesList.push_back(p.second.ref_name);
			}
		}
	}
	std::reverse(nodesList.begin(), nodesList.end());

	// Produce code for all needed nodes

	globals.clear();
	std::ostringstream out;

	for (const string &n : nodesList) {
		auto idx = std::find_if(nodes.begin(), nodes.end(),
				[&n](const Node &no) {
					return no.name == n;
				});
		if (idx == nodes.end())
			throw std::runtime_error(
					"This should not happen, because nodeList was compiled from nodes.");
		out << '\n';
		idx->GenerateCode(out, globals, images);
	}

	std::ostringstream code;
	code << "#version 330 core \n\n";

	std::vector<Variable> temp(globals.begin(), globals.end());
	std::sort(temp.begin(), temp.end());
	for (const auto &g : temp) {
		code << g.ToDeclaration() << '\n';
	}
	code << out.str();

	return code.str();
}

bool OpenGLMaterialX::Start() {
	if (!OpenGLShader::Start())
		return false;
	for (const auto &image : images) {
		image.second.BindToTextureUnit();
		SetUniformInt("uni_" + image.second.name, image.second.textureUnit);
	}
	return true;
}

