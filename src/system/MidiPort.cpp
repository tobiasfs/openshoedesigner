///////////////////////////////////////////////////////////////////////////////
// Name               : MidiPort.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.09.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "MidiPort.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

size_t MidiPort::instancecounter = 0;

MidiDevice::MidiDevice(const std::string &name) :
		name(name) {
	cc.fill(0);
	ccold.fill(0);
}

MidiDevice::~MidiDevice() {
	Close();
}

void MidiDevice::Close() {
#ifdef USE_PORTMIDI
	if (input != nullptr)
		Pm_Close(input);
	input = nullptr;
	if (output != nullptr)
		Pm_Close(output);
	output = nullptr;
#endif
}

bool MidiDevice::operator ==(const std::string &name) const {
	return name.compare(this->name) == 0;
}

bool MidiDevice::IsOpen() const {
#ifdef USE_PORTMIDI
	return (input != nullptr) || (output != nullptr);
#else
	return false;
#endif
}

bool MidiDevice::IsInput() const {
#ifdef USE_PORTMIDI
	return input != nullptr;
#else
	return false;
#endif
}

bool MidiDevice::IsOutput() const {
#ifdef USE_PORTMIDI
	return output != nullptr;
#else
	return false;
#endif
}

bool MidiDevice::IsBidirectional() const {
#ifdef USE_PORTMIDI
	return (input != nullptr) && (output != nullptr);
#else
	return false;
#endif
}

std::string MidiDevice::GetName() const {
	return name;
}

void MidiDevice::Push(uint8_t from, uint8_t to) {
	for (size_t n = from; n <= to; ++n)
		ccold[n] = cc[n] + 1;
}

bool MidiDevice::Poll() {
	bool flag = false;
#ifdef USE_PORTMIDI
	if (output != nullptr) {
		for (size_t n = 0; n < cc.size(); ++n) {
			if (cc[n] != ccold[n]) {
				buffer[0].message = Pm_Message(0xB0, n, cc[n]);
				buffer[0].timestamp = 0;
				Pm_Write(output, buffer, 1);
				ccold[n] = cc[n];
			}
		}
	}
	if (input != nullptr) {
		while (Pm_Poll(input)) {
			if (Pm_Read(input, buffer, 1) > 0) {
				uint8_t ch = Pm_MessageData1(buffer[0].message);
				uint8_t val = Pm_MessageData2(buffer[0].message);
				flag |= (val != cc[ch]);
				cc[ch] = ccold[ch] = val;
			}
		}
	}
#endif
	return flag;
}

void MidiDevice::Flush() {
#ifdef USE_PORTMIDI
	if (input == nullptr)
		return;
	//	Pm_SetFilter(midi, PM_FILT_CONTROL);
	while (Pm_Poll(input))
		Pm_Read(input, buffer, 1);
#endif
}

bool MidiDevice::PollEvent(uint8_t *data0, uint8_t *data1, uint8_t *data2) {
#ifdef USE_PORTMIDI
	if (input == nullptr)
		return false;
	if (!Pm_Poll(input))
		return false;
	if (Pm_Read(input, buffer, 1) > 0) {
		*data0 = Pm_MessageStatus(buffer[0].message);
		*data1 = Pm_MessageData1(buffer[0].message);
		*data2 = Pm_MessageData2(buffer[0].message);
		return true;
	}
#endif
	return false;
}

MidiPort::MidiPort() {
#ifdef USE_PORTMIDI
	status = pmNoError;
	length = pmNoData;
#endif

	instancecounter++;
#ifdef USE_PORTMIDI
	if (instancecounter == 1)
		Pm_Initialize();
#endif
}

MidiPort::~MidiPort() {
#ifdef USE_PORTMIDI
	CloseAll();

	if (instancecounter == 1)
		Pm_Terminate();
#endif
	assert(instancecounter > 0);
	instancecounter--;
}

inline bool operator==(const std::shared_ptr<MidiDevice> &device,
		const std::string &name) {
	return ((*device) == name);
}

std::shared_ptr<MidiDevice> MidiPort::Open(const std::string &name,
		Direction direction) {
	std::shared_ptr<MidiDevice> temp;
	auto it = std::find(devices.begin(), devices.end(), name);
	if (it == devices.end()) {
		temp = std::make_shared<MidiDevice>(name);
		devices.push_back(temp);
	} else {
		temp = *it;
	}

#ifdef USE_PORTMIDI
	PmDeviceID id = 0;
	const PmDeviceInfo *info = Pm_GetDeviceInfo(id);
	while (info != nullptr) {
		if (name.compare(info->name) == 0 && info->input
				&& (direction == Direction::Input
						|| direction == Direction::Bidirectional)) {
			if (temp->input == nullptr) {
				PmError err = Pm_OpenInput(&(temp->input), id, nullptr, 100, nullptr,
				nullptr);
				if (err != pmNoError) {
					std::cerr << "Error opening MIDI input: "
							<< DecodeError(err) << '\n';
					temp->input = nullptr;
				}
			}
		}
		if (name.compare(info->name) == 0 && info->output
				&& (direction == Direction::Output
						|| direction == Direction::Bidirectional)) {
			if (temp->output == nullptr) {
				PmError err = Pm_OpenOutput(&(temp->output), id, nullptr, 100,
				nullptr,
				nullptr, 0);
				if (err != pmNoError) {
					std::cerr << "Error opening MIDI output: "
							<< DecodeError(err) << '\n';
					temp->output = nullptr;
				}
			}
		}
		info = Pm_GetDeviceInfo(++id);
	}
	temp->Flush();
#endif
	return temp;
}

bool MidiPort::IsLastInstance() const {
	return (instancecounter == 1);
}

//void MidiPort::CycleLibrary()
//{
//	this->Close();
//	if(instancecounter == 1){
//#ifdef USE_PORTMIDI
//		Pm_Terminate();
//		Pm_Initialize();
//#endif
//	}else{
//		// More than one instance: This causes problems for the other instances that are unaware of this cycling.
//	}
//}

int MidiPort::GetDefaultInputDevice() const {
#ifdef USE_PORTMIDI
	return Pm_GetDefaultInputDeviceID();
#else
	return -1;
#endif
}

int MidiPort::GetDefaultOutputDevice() const {
#ifdef USE_PORTMIDI
	return Pm_GetDefaultOutputDeviceID();
#else
	return -1;
#endif
}

std::set<std::string> MidiPort::GetDeviceNames() const {
	std::set<std::string> temp;
#ifdef USE_PORTMIDI
	int idx = 0;
	const PmDeviceInfo *info = Pm_GetDeviceInfo(idx);
	while (info != nullptr) {
		temp.insert(info->name);
		++idx;
		info = Pm_GetDeviceInfo(idx);
	}
#endif
	return temp;
}

int MidiPort::GetDeviceCount() const {
	int temp = 0;
#ifdef USE_PORTMIDI
	while (Pm_GetDeviceInfo(temp) != nullptr)
		temp++;
#endif
	return temp;
}

std::string MidiPort::GetDeviceName(int nr) const {
#ifdef USE_PORTMIDI
	const PmDeviceInfo *temp = Pm_GetDeviceInfo(nr);
	if (temp == nullptr) {
		return std::string("");
	}
	std::string name = temp->name;
	return name;
#else
	return std::string("");
#endif
}

std::string MidiPort::GetDeviceInterfaceName(int nr) const {
#ifdef USE_PORTMIDI
	const PmDeviceInfo *temp = Pm_GetDeviceInfo(nr);
	if (temp == nullptr) {
		return std::string("");
	}
	std::string name = temp->interf;
	return name;
#else
	return std::string("");
#endif
}

bool MidiPort::IsDeviceInput(int nr) const {
#ifdef USE_PORTMIDI
	const PmDeviceInfo *temp = Pm_GetDeviceInfo(nr);
	if (temp == nullptr)
		return false;
	return temp->input;
#else
	return false;
#endif
}

bool MidiPort::IsDeviceOutput(int nr) const {
#ifdef USE_PORTMIDI
	const PmDeviceInfo *temp = Pm_GetDeviceInfo(nr);
	if (temp == nullptr)
		return false;
	return temp->output;
#else
	return false;
#endif
}

bool MidiPort::IsDeviceAvailable(int nr) const {
#ifdef USE_PORTMIDI
	const PmDeviceInfo *temp = Pm_GetDeviceInfo(nr);
	if (temp == nullptr)
		return false;
	return !temp->opened;
#else
	return false;
#endif
}

void MidiPort::Close(const std::string &name) {
	auto it = std::find(devices.begin(), devices.end(), name);
	if (it != devices.end())
		(*it)->Close();
}

void MidiPort::Close(size_t idx) {
	devices[idx]->Close();
}

void MidiPort::CloseAll() {
	for (auto &device : devices)
		device->Close();
}

std::shared_ptr<MidiDevice> MidiPort::GetFirstOpenDevice(Direction direction) {
#ifdef USE_PORTMIDI
	if (direction == Direction::Input) {
		for (auto &device : devices)
			if (device->input != nullptr)
				return device;
	}
	if (direction == Direction::Output) {
		for (auto &device : devices)
			if (device->output != nullptr)
				return device;
	}
	if (direction == Direction::Bidirectional) {
		for (auto &device : devices)
			if (device->input != nullptr && device->output != nullptr)
				return device;
	}
#endif
	return nullptr;
}

#ifdef USE_PORTMIDI
std::string MidiPort::DecodeError(PmError err) {
	return std::string(Pm_GetErrorText(err));
//	switch(err){
//	case pmNoError:
//		return "";
//	case pmHostError:
//		return "Host error.";
//	case pmInvalidDeviceId:
//		return "Invalid device id.";
//	case pmInsufficientMemory:
//		return "Insufficient memory.";
//	case pmBufferTooSmall:
//		return "Buffer too small.";
//	case pmBufferOverflow:
//		return "Buffer overflow.";
//	case pmBadPtr:
//		return "Bad pointer.";
//	case pmInternalError:
//		return "Internal error.";
//	case pmBufferMaxSize:
//		return "Max buffer size.";
//	default:
//		return "Unknown error.";
//	}
//	return "";
}
#endif

std::string MidiPort::ToString() const {
	std::ostringstream buf;
#ifdef USE_PORTMIDI
	int idx = 0;
	const PmDeviceInfo *info = Pm_GetDeviceInfo(idx);
	while (info != nullptr) {
		buf << "#" << idx << " ";
		buf << "Name: \"" << info->name << "\" ";
		buf << "Interface: \"" << info->interf << "\" ";
		buf << "I:" << info->input << " O:" << info->output;
		buf << "\n";
		++idx;
		info = Pm_GetDeviceInfo(idx);
	}
#endif
	return buf.str();
}

