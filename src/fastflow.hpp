#include <sstream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "Timer.hpp"
#include "Utils.hpp"


long fastflow(Stream* inputStream, bool v, unsigned nw);


struct emitter_task: ff::ff_node_t<Window> {
  bool v;
  Stream* stream;
  emitter_task(bool verbose, Stream* inputStream): v(verbose), stream(inputStream) {}

  Window* svc(Window* window) {
    for (unsigned long i = 0; i < stream->w_no(); i++) {
      window = stream->pickWindow(i);
      if (v) {
        std::stringstream message;
        message << "[Emitter] Window " << i << ": "
                << Utils::serializeWindow(window) << std::endl;
        std::cout << message.str();
      }
      ff_send_out(window);
    }
    return EOS;
  }
};


struct worker_task: ff::ff_node_t<Window> {
  bool v;
  worker_task(bool verbose): v(verbose) {}

  Skyline* svc(Window* window) {
    if (v) {
      std::stringstream message;
      message << "[Worker" << get_my_id() << "] Window " << window->id() << ": "
              << Utils::serializeWindow(window) << std::endl;
      std::cout << message.str();
    }

    auto skyline = Utils::processWindow(window);

    if (v) {
      std::stringstream message;
      message << "[Worker" << get_my_id() << "] Skyline " << skyline->id() << ": "
              << Utils::serializeWindow(skyline) << std::endl;
      std::cout << message.str();
    }

    return skyline;
  }
};


struct collector_task: ff::ff_node_t<Skyline> {
  bool v;
  collector_task(bool verbose): v(verbose) {}

  Skyline* svc(Skyline* skyline) {
    if (v) {
      std::stringstream message;
      message << "[Collector] Skyline " << skyline->id() << ": "
              << Utils::serializeWindow(skyline) << std::endl;
      std::cout << message.str();
    }
    delete skyline;
    return GO_ON;
  }
};


long fastflow(Stream* inputStream, bool v, unsigned nw) {
  Timer timer("Fastflow");

  auto emitter = ff::make_unique<emitter_task>(v, inputStream);
  auto collector = ff::make_unique<collector_task>(v);

  std::vector<std::unique_ptr<ff::ff_node>> workers;
  for (unsigned i = 0; i < nw; i++)
    workers.push_back(ff::make_unique<worker_task>(v));

  ff::ff_Pipe<> pipe(
    emitter,
    ff::make_unique<ff::ff_Farm<Window>>(std::move(workers)),
    collector
  );
  if (pipe.run_and_wait_end() < 0)
    ff::error("running pipe");

  return timer.getTime();
}