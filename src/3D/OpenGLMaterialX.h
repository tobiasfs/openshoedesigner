///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLMaterialX.h
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
#ifndef L3D_OPENGLMATERIALX_H_
#define L3D_OPENGLMATERIALX_H_

/** \class OpenGLMaterialX
 * 	\code #include "OpenGLMaterialX.h"\endcode
 * 	\ingroup OpenGL
 *  \brief Read a zipped MaterialX file provide an OpenGL shader.
 *
 * [MaterialX.v1.38.Spec](https://materialx.org/assets/MaterialX.v1.38.Spec.pdf)
 * or
 * [MaterialX: An Open Standard for Network-Based CG Object Looks, Version 1.39](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/documents/Specification/MaterialX.Specification.md)
 *
 * Substance Painter can export materials as MTLX files. These files are .zip
 * files that contain a .mtlx file and a set of PNG or JPEG files.
 *
 * ### Sources for textures
 *
 *  * <https://ambientcg.com/>
 *
 * ### References
 *
 *  * <https://github.com/AcademySoftwareFoundation/MaterialX/tree/main/libraries/pbrlib/genglsl>
 *
 */

#include "OpenGL.h"

#include "OpenGLShader.h"

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <filesystem>
#include <map>
#include <regex>
#include <set>
#include <vector>

class OpenGLMaterialX: public OpenGLShader {
public:
	/**\brief Texture image
	 *
	 * Reads JPG or PNG images and stores them in RAM.
	 */
	class Image {
	public:
		Image() = delete;
		Image(const std::filesystem::path &filename_);
		virtual ~Image();

		void LoadFromString(std::string &data);
		/**\brief Loads the image-data into the GPU.
		 *
		 *  Copies the image into the GPU, assign a texture and assigns the
		 *  texture to a TextureUnit (there are at least 16 units).
		 *
		 * \return index of the TextureUnit
		 */
		GLuint LoadIntoGPU();

		/**\brief Flip the Y axis of the image.
		 *
		 */
		void FlipY();

		/**\brief BindToTextureUnit a texture already in GPU to its texture-unit.
		 */
		void BindToTextureUnit() const;

		std::string name;
		std::filesystem::path filename;
		std::vector<uint8_t> data;
		unsigned int bitsPerComponent = 0;
		unsigned int componentsPerPixel = 0;
		unsigned int w = 0;
		unsigned int h = 0;
		GLuint textureID = 0;
		GLuint textureUnit = 0;
	};
	class Variable {
	public:
		enum Direction {
			VAR, PARAM, IN, OUT, UNIFORM
		};
		Variable(Direction dir_, std::string type_, std::string name_,
				int location_ = -1);
		std::string ToDeclaration() const;
		std::string ToDefinition() const;
		std::string operator()() const;

		bool operator<(const Variable &rhs) const;

		Direction dir = Direction::VAR;
		std::string type;
		std::string name;
		int location = -1;
		bool valid = false;
	};
	class Port {
	public:
		std::string type;
		std::string name;

		std::string text;
		std::string ref_name;

		OpenGLMaterialX::Variable ToDefinition(std::ostream &out,
				const std::string &param = "", const std::string &requiredType =
						"") const;
		std::string ToValue(const std::string &param = "",
				const std::string &requiredType = "") const;
	};
	class Node {
	public:
		Node() = default;
		std::string name;
		std::string type;
		std::string datatype;
		std::map<std::string, Port> inputs;
		std::map<std::string, Port> outputs;
		std::map<std::string, Node> nodes;
		bool HasInput(const std::string &inp) const;
		OpenGLMaterialX::Variable InputToString(std::ostream &out,
				const std::string &name_, const std::string &type_,
				const std::string &param_, const std::string &default_) const;
		void GenerateCode(std::ostream &out, std::set<Variable> &globals,
				std::map<std::filesystem::path, Image> &images_);
	};

public:
	OpenGLMaterialX() = default;
	virtual ~OpenGLMaterialX() = default; //FIXME: ==47976== Jump to the invalid address stated on the next line
						    // ==47976==    at 0x63B6F20: ???
						    // ==47976==    by 0x13410D: std::pair<std::filesystem::__cxx11::path const, OpenGLMaterialX::Image>::~pair()

public:
	/**\brief Load a MATX file from a zipped archive.
	 *
	 * \param filename_ .zip-file with .matx and textures
	 */
	void Load(const std::filesystem::path &filename_);
private:
	void ParseXML(std::sregex_iterator &xml_it);
	void ParseXMLNode(std::sregex_iterator &xml_it, Node &node);
	void ParseNodeAttributes(std::sregex_iterator &xml_it, Node &node);
	void ParsePortAttributes(std::sregex_iterator &xml_it, Port &port);

public:
	/**\brief Start the shader and bind the textures
	 *
	 * \return Boolean if the shader could start (i.e. no compilation errors)
	 */
	virtual bool Start();

	/**\brief Write the shader code.
	 *
	 * \return std::string with shader code.
	 */
	std::string GenerateFragmentShader();

private:
	static std::string ToGlslType(const std::string &type);

public:
	std::vector<Node> nodes;
	std::set<Variable> globals;
	std::map<std::filesystem::path, Image> images;
};

#endif /* L3D_OPENGLMATERIALX_H_ */
