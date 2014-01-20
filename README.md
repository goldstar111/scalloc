# scalloc

A Fast, Multicore-Scalable, Low-Memory-Overhead Allocator

Scalloc is a new concurrent memory allocator that actively utilizes recently
developed efficient and scalable concurrent data structures. Global concurrent
data structures have long been considered performance and scalability
bottlenecks. Hence, many existing allocators aim at minimizing the use of such.
Scalloc, in contrast, freely uses global data structures for reusing memory and
benefits of the existence of new efficient and scalable ones. In terms of
performance and scalability, scalloc is comparable to the best existing
allocators. In terms of memory consumption, it outperforms the best scalable
allocators. The improvement in memory consumption is enabled by the efficiency
of the global data structures, which let scalloc do more work in less time.
Scalloc also borrows commonly used concepts in memory management like size-class
partitioned memory and private heaps. In addition, scalloc introduces a new idea
of virtual spans for uniform treatment of small and big objects without
increased memory fragmentation.

## Binaries

We do not explicitly provide pre-built binaries. However, binaries for Linux
(x86\_64) are generated by our continues integration builds and can be found on
the [scalloc drone.io downloads
page](https://drone.io/github.com/cksystemsgroup/scalloc/files). Note that since
these binaries get generated on every code change, they may not be very stable.

## Building [![Build Status](https://drone.io/github.com/cksystemsgroup/scalloc/status.png)](https://drone.io/github.com/cksystemsgroup/scalloc/latest)

We support building on OS X (>10.6) and Linux-based systems using
[gyp](https://code.google.com/p/gyp/). The build setup can also be accessed on
[drone.io](https://drone.io/github.com/cksystemsgroup/scalloc/admin).

### Setting up a build environment

Checkout the latest release version

    git clone https://github.com/cksystemsgroup/scalloc.git
    cd scalloc

We suggest building from the `release` tag, which should be more stable than the
current HEAD. This step is optional.

    git checkout release

If you don't have a global gyp installation, you can get a local one using

    tools/make_deps.sh
    
Then, generate a build environment

    build/gyp/gyp --depth=. scalloc.gyp

The build supports some compile time parameters:

* san_reuse_threshold: Treshold for stealing spans from other threads. (default: 80)
* local_reuse_threshold: Threshold for reusing local spans. (default: 80)
* log_level: One of kTrace, kWarning, kFatal, ... (default: kWarning)
* small_space: Size of small allocator space.
* eager_madvise_threshold: Threshold for eager madvising on returning spans.

Variables can be set with `-D variable=value` on the `gyp` command.

### Building on Linux

    BUILDTYPE=Release make

### Building on OSX

Open `scalloc.xcodeproj` and build it using Xcode, or

    build/gyp/gyp --depth=. scalloc.gyp --build=Release

## Using scalloc

### ... on Linux

In order to make use of scalloc, preload it using `LD_PRELOAD`.

    LD_PRELOAD=/path/to/libscalloc.so ./foo

scalloc heavily makes use of 64bit address space. If you run into some limits
you probably need to disable overcommit accounting. On recent versions of Linux
you can do this by

    sudo sh -c "echo 1 > /proc/sys/vm/overcommit_memory"

See the [kernel docs](https://www.kernel.org/doc/Documentation/vm/overcommit-accounting)
for more information.

### ... on OSX

Similar to preloading on Linux, one can preload scalloc using

    DYLD_INSERT_LIBRARIES=/path/to/libscalloc.dylib DYLD_FORCE_FLAT_NAMESPACE=1 ./foo

## License

Copyright (c) 2012-2013, the scalloc Project Authors.
All rights reserved. Please see the AUTHORS file for details.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the scalloc Project.
