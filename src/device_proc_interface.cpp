#include "device_proc_interface.hpp"

void Device::acquire(std::shared_ptr<Process> owner) {
	this->mtx.lock();
	this->owner = owner;
	std::cout << "Device " << id << " aquired by " << owner->get_pname() << " with pid " << owner->get_pid() << "\n";
}

void Device::release() {
	this->mtx.unlock();
	std::cout << "Device " << id << " released by " << owner->get_pname() << " with pid " << owner->get_pid() << "\n";
	this->owner = nullptr;
}


void Process::acquire_device(std::shared_ptr<Device>& device) {
	device->acquire(std::shared_ptr<Process>(this));
	acquired_devices.insert({ device->id, device });
}
void Process::release_device(std::shared_ptr<Device>& device) {
	device->release();
	acquired_devices.erase(device->id);
}


std::string Process::get_pname() { return pname; }
int Process::get_pid() { return pid; }
void Process::print_aquired_devices() {
	std::cout << "Device ID" << std::endl;
	for (auto ele : acquired_devices) {
		std::cout << ele.first << std::endl;
	}
}

void Process::init_process(std::function<void()> job) {
	this->job = job;
}

void Process::start_process() {
	job();
}
