// Copyright (c) 2014, the scalloc Project Authors.  All rights reserved.
// Please see the AUTHORS file for details.  Use of this source code is governed
// by a BSD license that can be found in the LICENSE file.

#ifndef SCALLOC_DISTRIBUTED_QUEUE_H_
#define SCALLOC_DISTRIBUTED_QUEUE_H_

#include <cstdlib>

#include "common.h"
#include "locked_stack-inl.h"  // Backend: LockedStack
#include "random.h"
#include "stack-inl.h"  // Backend: Stack
#include "typed_allocator.h"

#define MAX_BACKENDS 120

#ifndef BACKEND_TYPE
#define BACKEND_TYPE Stack
#endif  // BACKEND_TYPE

namespace scalloc {

class DistributedQueue {
 public:
  typedef BACKEND_TYPE Backend;

  // Upper bound on DQ backends (and thus also State records).
  static const size_t kMaxBackends = MAX_BACKENDS;

  struct State {
    uint64_t backend_states[kMaxBackends];
  };

  static void Init(TypedAllocator<Backend>* backend_alloc);

  inline DistributedQueue() {}
  void Init(size_t p);
  void Enqueue(void* p);
  void EnqueueAt(void* p, const size_t backend_id);
  void* Dequeue();
  void* DequeueOnlyAt(const size_t backend_id);
  void* DequeueStartAt(const size_t first_backend_id);

 private:
  static scalloc::TypedAllocator<Backend>* backend_allocator_;

  // Number of backends.
  size_t p_;

  // The actual DQ backends.
  Backend* backends_[kMaxBackends];

  DISALLOW_ALLOCATION();
  DISALLOW_COPY_AND_ASSIGN(DistributedQueue);
};


inline void DistributedQueue::Enqueue(void* p) {
  size_t start = static_cast<size_t>(hwrand()) % p_;
  EnqueueAt(p, start);
}


inline void DistributedQueue::EnqueueAt(void* p, const size_t backend_id) {
  backends_[backend_id % p_]->Put(p);
}


inline void* DistributedQueue::Dequeue() {
  size_t start = static_cast<size_t>(hwrand()) % p_;
  return DequeueStartAt(start);
}


inline void* DistributedQueue::DequeueOnlyAt(const size_t backend_id) {
  return backends_[backend_id % p_]->Pop();
}


inline void* DistributedQueue::DequeueStartAt(const size_t first_backend_id) {
  void* result;
  size_t start = first_backend_id % p_;
  State state;
  size_t i;
  while (true) {
 GET_RETRY:
    for (size_t _cnt = 0; _cnt < p_; _cnt++) {
      i = (_cnt + start) % p_;
      if ((result = backends_[i]->PopRecordState(
              &(state.backend_states[i]))) != NULL) {
        return result;
      }
    }
#ifdef DQ_NON_LIN_EMPTY
    return NULL;
#endif  // DQ_NON_LIN_EMPTY
    for (size_t _cnt = 0; _cnt < p_; _cnt++) {
      i = (_cnt + start) % p_;
      if (state.backend_states[i] != backends_[i]->GetState()) {
        start = i;
        goto GET_RETRY;
      }
    }
    return NULL;
  }
}

}  // namespace scalloc

#endif  // SCALLOC_DISTRIBUTED_QUEUE_H_
