#pragma once
#include<iostream>
#include<mutex>
#include<vector>
#include<map>
#include<functional>

class Device;
class Process;

class Printer {
private:
	std::mutex mtx;
	static void __print() { return; };
	template<typename T, typename ...Args> void __print(T val, Args ...args) {
		std::cout << val << " ";
		__print(args...);
	}
public:
	template<typename ...Args> void print(Args ...args) {
		std::lock_guard<std::mutex> lock(mtx);
		__print(args...);
	}
	template<typename ...Args> void println(Args ...args) {
		std::lock_guard<std::mutex> lock(mtx);
		__print(args...);
	}
};

class Process {
private:
	int pid;
	std::string pname;
	std::function<void()> job;
	std::map<int, std::shared_ptr<Device>> acquired_devices;
public:
	void acquire_device(std::shared_ptr<Device>& device);
	void release_device(std::shared_ptr<Device>& device);
	std::string get_pname();
	int get_pid();
	void init_process(std::function<void()> job);
	void start_process();
	void print_aquired_devices();

	Process() = default;
	Process(int pid, const std::string& pname, const std::function<void()>& job = {}, const std::map<int, std::shared_ptr<Device>>& acquired_devices = {})
		: pid(pid), pname(pname), job(job), acquired_devices(acquired_devices) {}
};

class Device {
public:
	int id;
	std::mutex mtx;
	std::shared_ptr<Process> owner;
	Device(int id) : id(id) {}
	Device(int id, std::shared_ptr<Process> owner) : id(id), owner(owner) {}
	void acquire(std::shared_ptr<Process> owner);
	void release();
};