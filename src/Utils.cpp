#include "Utils.hpp"

std::string Utils::serializeTuple(Tuple* tuple) {
  std::string string = "<";
  for (unsigned long i = 0; i < tuple->size(); i++)
    string += std::to_string(tuple->at(i)) + (i != tuple->size() - 1 ? "," : ">");
  return string;
}

std::string Utils::serializeWindow(std::vector<Tuple*>* window) {
  std::string string = "[";
  for (unsigned long i = 0; i < window->size(); i++)
    string += serializeTuple(window->at(i)) + (i != window->size() - 1 ? ", " : "]");
  return string;
}

short Utils::compareTuple(Tuple* ti, Tuple* tj) {
  unsigned long k = 0;
  bool allLT = true;
  bool allGT = true;
  // Scan each component, stop if them don't eliminate themselves:
  while (k < ti->size() && (allLT || allGT)) {
    // if is bigger, this tuple has not each component less than the other tuple;
    if (ti->at(k) > tj->at(k)) {
      allLT = false;
      k++;
    }
      // if is smaller, this tuple has not each component greater than the other tuple;
    else if (ti->at(k) < tj->at(k)) {
      allGT = false;
      k++;
    }
      // else them are equals, so is neither GT nor LT.
    else {
      allLT = false;
      allGT = false;
    }
  }
  if (allLT) return -1;
  else if (allGT) return 1;
  return 0;
}


// FIXME: wrong skyline
Skyline* Utils::processWindow(Window* window) {
  // save which tuple keep and which not
  std::vector<bool> keep(window->size());
  for (unsigned long i = 0; i < keep.size(); i++)
    keep[i] = true;

  // foreach tuple check if survive and which tuple eliminates
  for (unsigned long i = 0; i < window->size(); i++) {
    unsigned long j = i + 1;
    // Compares it with the subsequent elements.
    // Continue while is not deleted, since in that case all the tuple that this one will delete
    // are already deleted or will be deleted by the one that have deleted this one.
    while (keep[i] && j < window->size()) {
      if (!keep[j]) continue; // already deleted, continue.
      // checking if each tuple component of the first tuple is less or greater than
      // the corresponding component of the other tuple.
      short cmp = compareTuple(window->at(i), window->at(i));
      // If the first tuple has each component strictly less than the corresponding component of the second one,
      // the first tuple eliminates the second one.
      if (cmp < 0) keep[j] = false;
      // Vice-versa, if each component of the first one is strictly greater than the corresponding one
      // of the second tuple, the second tuple eliminates the first one.
      else if (cmp > 0) keep[i] = false;
      // Else, them remains both on the window, since them are still candidates for the skyline.
      j++;
    }
  }

  // count the survived
  auto survived_count = 0;
  for (unsigned long i = 0; i < keep.size(); i++)
    if (keep[i]) survived_count++;
  
  // build the skyline using the bit mask vector
  auto skyline = new Skyline(survived_count, window->id());
  auto si = 0;
  for (unsigned long wi = 0; wi < window->size(); wi++)
    if (keep[wi]) {
      skyline->at(si) = window->at(wi);
      si++;
    }

  // the window can be deallocated, but not its tuples
  delete window;

  return skyline;
}
