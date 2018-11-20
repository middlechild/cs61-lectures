#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <thread>
#include <algorithm>
#include <mutex>

struct bbuffer {
    static constexpr size_t bcapacity = 128;
    char bbuf_[bcapacity];
    size_t bpos_ = 0;
    size_t blen_ = 0;
    bool write_closed_ = false;
    std::mutex mutex_;

    ssize_t read(char* buf, size_t sz);
    ssize_t write(const char* buf, size_t sz);
    void shutdown_write();
};

ssize_t bbuffer::write(const char* buf, size_t sz) {
    std::unique_lock<std::mutex> guard(this->mutex_);
    assert(!this->write_closed_);
    size_t pos = 0;
    while (pos < sz && this->blen_ < bcapacity) {
        size_t bindex = (this->bpos_ + this->blen_) % bcapacity;
        size_t bspace = std::min(bcapacity - bindex, bcapacity - this->blen_);
        size_t n = std::min(sz - pos, bspace);
        memcpy(&this->bbuf_[bindex], &buf[pos], n);
        this->blen_ += n;
        pos += n;
    }
    if (pos == 0 && sz > 0) {
        return -1;  // try again
    } else {
        return pos;
    }
}

ssize_t bbuffer::read(char* buf, size_t sz) {
    std::unique_lock<std::mutex> guard(this->mutex_);
    size_t pos = 0;
    while (pos < sz && this->blen_ > 0) {
        size_t bspace = std::min(this->blen_, bcapacity - this->bpos_);
        size_t n = std::min(sz - pos, bspace);
        memcpy(&buf[pos], &this->bbuf_[this->bpos_], n);
        this->bpos_ = (this->bpos_ + n) % bcapacity;
        this->blen_ -= n;
        pos += n;
    }
    if (pos == 0 && sz > 0 && !this->write_closed_) {
        return -1;  // try again
    } else {
        return pos;
    }
}

void bbuffer::shutdown_write() {
    std::unique_lock<std::mutex> guard(this->mutex_);
    this->write_closed_ = true;
}


void writer_threadfunc(bbuffer& bb) {
    // Write `Hello world!\n` to the buffer a million times.
    // Result should have 13000000 characters.
    const char message[] = "Hello world!\n";
    const size_t message_len = strlen(message);
    for (int i = 0; i != 1000000; ++i) {
        size_t pos = 0;
        while (pos < message_len) {
            ssize_t nw = bb.write(&message[pos], message_len - pos);
            if (nw > -1) {
                pos += nw;
            }
        }
    }
    bb.shutdown_write();
}

void reader_threadfunc(bbuffer& bb) {
    // Read from the buffer until closed and write to stdout.
    char buf[BUFSIZ];
    ssize_t nr;
    while ((nr = bb.read(buf, sizeof(buf))) != 0) {
        if (nr > -1) {
            fwrite(buf, 1, nr, stdout);
        }
    }
}


int main() {
    bbuffer bb;
    std::thread reader(reader_threadfunc, std::ref(bb));
    std::thread writer(writer_threadfunc, std::ref(bb));
    reader.join();
    writer.join();
}
