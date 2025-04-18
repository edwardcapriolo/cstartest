#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/future-util.hh>
#include <seastar/net/api.hh>
#include <iostream>

seastar::future<int> slow() {
    using namespace std::chrono_literals;
    return seastar::sleep(100ms).then([] {
    		std::cout << "future exec\n";
    		return 3; });
}

seastar::future<> f() {
    std::cout << "Sleeping... " << std::flush;
    using namespace std::chrono_literals;
    for (int i = 0; i < 100; i++){
    	slow();
    }
    return seastar::sleep(1s).then([] {
        std::cout << "Done.\n";
    });
}
/*
seastar::future<int> recompute_number(int number){
	return seastar::make_ready_future<int>(3);
}

seastar::future<> push_until_100(seastar::lw_shared_ptr<std::vector<int>> queue, int element) {
	return seastar::do_until( [queue] { return queue->size() == 100; }, [queue, element] {
		return recompute_number(element).then( [queue] (int new_element) {
			queue -> push_back(new_element);
		});
	});
}
seastar::future<> g() {
	seastar::lw_shared_ptr<std::vector<int>> queue;
	push_until_100(queue, 5);
	std::cout << queue.size();
	return seastar::make_ready_future<>();
} */


seastar::future<> service_loop() {
	return seastar::do_with(seastar::listen(seastar::make_ipv4_address({1234})), 
			[] (auto& listener) {
		return seastar::keep_doing([&listener] () {
			return listener.accept().then(
			 [] (seastar::accept_result res) {
				std::cout << "Accept connection from " << res.remote_address << "\n";
			});
		});
	});
}

seastar::future<> h() {
	return seastar::parallel_for_each( std::views::iota(0u , seastar::smp::count), 
		[] (unsigned c) {
			return seastar::smp::submit_to(c, service_loop);
	});
}

/*
int main(int argc, char** argv) {
  seastar::app_template app;
  app.run(argc, argv, [] {   
    std::cout << "Jello world\n";
    using namespace std::chrono_literals; 
    return seastar::sleep(1s).then([] {
      std::cout << "I slept then continued\n";
    }); 
  });
}*/
int main(int argc, char** argv) {
    seastar::app_template app;
    try {
        app.run(argc, argv, h);
    } catch(...) {
        std::cerr << "Couldn't start application: "
                  << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}


