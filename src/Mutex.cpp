// Mutex.cpp : Defines the entry point for the application.
//
#include "device_proc_interface.hpp"
int main() {
	std::shared_ptr<Device> mouse{ new Device(1) };
	std::shared_ptr<Device> keyboard{ new Device(2) };
	
	Process p1(1, "process1");
	Process p2(2, "process2");

	Printer printer;

	p1.init_process([&]() -> void {
		p1.acquire_device(mouse);
		printer.println("P1 thread id : ", std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::duration<int>(2));
		p1.acquire_device(keyboard);
		std::this_thread::sleep_for(std::chrono::duration<int>(2));
		p1.release_device(mouse);
		p1.release_device(keyboard);
	});
	
	p2.init_process([&]() -> void {
		p2.acquire_device(keyboard);
		printer.println("P2 thread id : ",std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::duration<int>(2));
		p2.acquire_device(mouse);
		std::this_thread::sleep_for(std::chrono::duration<int>(2));
		p2.release_device(keyboard);
		p2.release_device(mouse);
	});

	printer.println("Main thread id : ", std::this_thread::get_id());
	std::thread p1_thread([&]() { p1.start_process(); }),
		p2_thread([&]() { p2.start_process(); });
	p1_thread.join();
	p2_thread.join();
	return 0;
}