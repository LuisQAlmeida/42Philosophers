# Tutorial References

These references are development-era learning material. They are not the
authoritative documentation for the maintained implementation.

## SRC-001

- **Title:** Introduction To Threads (pthreads) | C Programming Tutorial
- **Creator:** Portfolio Courses
- **Platform:** YouTube
- **Link:** https://www.youtube.com/watch?v=ldJ8WGZVXZk
- **Accessed:** 2026-03-23
- **Notes:** [`notes/src_001_pthreads_intro_notes.md`](notes/src_001_pthreads_intro_notes.md)
- **Relevance:** Introductory resource for thread creation, joining and argument
  passing with POSIX threads.

## SRC-002

- **Title:** Detached Threads (pthreads) | C Programming Tutorial
- **Creator:** Portfolio Courses
- **Platform:** YouTube
- **Link:** https://www.youtube.com/watch?v=p142bNjIsms
- **Accessed:** 2026-03-30
- **Notes:** [`notes/src_002_detached_threads_notes.md`](notes/src_002_detached_threads_notes.md)
- **Relevance:** Study material comparing joinable and detached pthreads. The
  maintained Philosophers implementation uses joinable philosopher threads and
  does not use `pthread_detach`.
