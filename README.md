BunnyBSD
======

BunnyBSD is a free, fast, secure, and highly portable Unix-like Open
Source operating system based on NetBSD.  It is available for a [wide range of
platforms](https://wiki.NetBSD.org/ports/), from large-scale servers
and powerful desktop systems to handheld and embedded devices.

Building
--------

You can cross-build BunnyBSD from most UNIX-like operating systems.
To build for amd64 (x86_64), in the src directory:

    ./build.sh -U -u -j4 -m amd64 -O ~/obj release

Additional build information available in the [BUILDING](BUILDING) file.

Testing
-------

On a running BunnyBSD system:

    cd /usr/tests; atf-run | atf-report
License & Attribution
---------------------

BunnyBSD is a fork of the [NetBSD](https://www.NetBSD.org/) operating system.
We are deeply grateful to **The NetBSD Foundation, Inc.** and all upstream
contributors for their decades of incredible work on building a clean, highly 
portable operating system.

* **Trademark Notice:** NetBSD® is a registered trademark of The NetBSD Foundation, Inc.
* **Licensing:** The base system of BunnyBSD is distributed under various 
  permissive open-source licenses, primarily the **2-clause and 3-clause BSD licenses**.
* **Copyrights:** Original upstream code remains copyright of **The NetBSD 
  Foundation, Inc.**, **The Regents of the University of California**, and 
  their respective contributors. New modifications, frameworks, and 
  BunnyBSD-exclusive code are licensed under the **2-clause BSD license** 
  and are Copyright (c) 2026 the BunnyBSD Project.

For the full license texts, disclaimers, and individual copyright notices, 
please refer to the [LICENSE](LICENSE) file in the root of the source tree.

Additional Links
----------------

- [NetBSD](https://www.NetBSD.org/)
- [NetBSD Source](https://github.com/NetBSD/src)
