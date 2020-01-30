#include <sstream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "Timer.hpp"
#include "Utils.hpp"



struct firstStage: ff::ff_node_t<long> {
  long* svc(long* item) {
    for (long i = 0; i < 10; i++) {
      std::stringstream message;
      message << "FirstStage" << get_my_id() << ": " << i << std::endl;
      std::cout << message.str();
      ff_send_out(new long(i));
    }
    return EOS;
  }
};

long* secondStage(long* item, ff::ff_node* const node) {
  std::stringstream message;
  message << "SecondStage" << node->get_my_id() << ": " << *item << std::endl;
  std::cout << message.str();
  return item;
}

struct thirdStage: ff::ff_node_t<long> {
  long* svc(long* item) {
    std::stringstream message;
    message << "ThirdStage" << get_my_id() << ": " << *item << std::endl;
    std::cout << message.str();
    delete item;
    return GO_ON;
  }
};




Tuple* newTuple(int t) {
  auto tuple = new Tuple(t);
  for (int i = 0; i < t; i++) {
    auto r = rand() % 100;
    tuple->at(i) = r;
  }
  return tuple;
}

long fastflow(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw);

long fastflow(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw) {
  // timer
  Timer timer("Fastflow");

  typedef std::vector<int> Tuple;
  typedef std::vector<Tuple*> Stream;

  // INPUT STREAM
  auto inputStream = new Stream(l);
  ff::ParallelFor pf;
  pf.parallel_for(0, l, [&inputStream, &t](const long i) {
    auto tuple = newTuple(t);
    inputStream->at(i) = tuple;
  });


  std::vector<std::unique_ptr<ff::ff_node>> farm;
  for (unsigned i = 0; i < nw; i++)
    farm.push_back(ff::make_unique<ff::ff_node_F<long>>(secondStage));

  ff::ff_Pipe<> pipe(
    ff::make_unique<firstStage>(),
    ff::make_unique<ff::ff_Farm<long>>(std::move(farm)),
    ff::make_unique<thirdStage>()
  );
  if (pipe.run_and_wait_end() < 0)
    ff::error("running pipe");

  // return time spent for autopilot
  return timer.getTime();
}