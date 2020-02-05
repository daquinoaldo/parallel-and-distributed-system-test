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
  bool all_j_let_i = true;
  bool all_j_get_i = true;
  bool one_j_lt_i = false;
  bool one_j_gt_i = false;
  // scan each component until there is hope for one of the tuple to dominate the other
  while (k < ti->size() && (all_j_let_i || all_j_get_i)) {
    // if there is an element of the tuple j greater than the corresponding element of the tuple i:
    // not all the element of j are lower or equal than the corresponding element of the tuple i
    // there is an the element of j greater than the corresponding element of the tuple i
    if (tj->at(k) > ti->at(k)) {
      all_j_let_i = false;
      one_j_gt_i = true;
    }
    // vice-versa
    if (tj->at(k) < ti->at(k)) {
      all_j_get_i = false;
      one_j_lt_i = true;
    }
    k++;
  }
  // i dominates j if all the j are lower or equal than the corresponding i
  // and at least one j is strictly lower than the corresponding i
  if (all_j_let_i && one_j_lt_i) return -1;
  // vice-versa
  if (all_j_get_i && one_j_gt_i) return 1;
  // else no tuple dominates
  return 0;
}


Skyline* Utils::processWindow(Window* window) {
  // save which tuple keep and which not
  std::vector<bool> keep(window->size());
  for (unsigned long i = 0; i < keep.size(); i++)
    keep[i] = true;
  
  // compare tuple and check which survive
  for (unsigned long i = 0; i < window->size(); i++) {
    unsigned long j = i + 1;
    // Compares it with the subsequent elements.
    // Continue while is not deleted, since in that case all the tuple that this one will delete
    // are already deleted or will be deleted by the one that have deleted this one.
    while (keep[i] && j < window->size()) {
      if (!keep[j]) {
        // already deleted, continue.
        j++;
        continue;
      }
      short cmp = compareTuple(window->at(i), window->at(j));
      if (cmp < 0) keep[j] = false;  // i dominates
      if (cmp > 0) keep[i] = false;  // j dominates
      // else, them remains both on the window, since them are still candidates for the skyline.
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
