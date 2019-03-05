#include <iostream>
#include <list>
#include <sstream>
#include "monitor.h"

#define con_count 5
#define buf_len 10

#define test 1

#if test == 1//production>>consumption

#define pro_count 20

#define con_time 10
#define pro_time 3

#elif test == 2//production<<consumption

#define pro_count 3

#define con_time 2
#define pro_time 11


#else//production==consumption

#define pro_count 5

#define con_time 5
#define pro_time 5

#endif

pthread_t producers[pro_count], consumers[con_count];

void *producer_fun(void *t);

void *consumer_fun(void *t);

class queue : Monitor {
    friend class Prod_Cons;

    Condition empty, full;

    std::string my_print(size_t i) {
        std::ostringstream ss, cc;
        ss << i;
        cc << fifo.size();
        std::string out = "queue " + ss.str() + " [" + cc.str() + "]: ";
        for (auto iter:fifo) {
            std::ostringstream kk;
            kk << iter;
            out += kk.str() + ", ";
        }
        out += "\n--------------------------------------------------------------------\n";
        return out;
    }
    std::list<size_t> fifo;

    bool producer(size_t producer_number, size_t queue, bool force_enter) {
        int item;
        enter();
        if (force_enter || fifo.size() < buf_len) {
            item = rand() % 89 + 10;
            if (fifo.size() == buf_len) { wait(full); }
            std::ostringstream ss, cc, kk;
            ss << producer_number;
            cc << item;
            kk << queue;
            std::string out = "producer " + ss.str() + " produced: " + cc.str() + " for queue: " + kk.str() + "\n";
            fifo.push_front((unsigned int) item);
            out += my_print(queue);
            std::cout << out;
            if (fifo.size() == 1) { signal(empty); }
            leave();
            sleep((unsigned int) (rand() % pro_time + 1));
            return true;
        } else {
            leave();
            return false;
        }


    }

    void consumer(size_t consumer_number) {
        enter();
        if (fifo.empty()) { wait(empty); }
        std::string out = "consumer ";
        std::ostringstream ss, cc;
        ss << consumer_number;
        cc << fifo.front();
        out += ss.str() + " consumed " + cc.str() + "\n";
        fifo.pop_back();
        out += my_print(consumer_number);
        std::cout << out;
        if (fifo.size() == buf_len - 1) { signal(full); }
        leave();
        sleep((unsigned int) (rand() % con_time + 1));
    }
};

class Prod_Cons {
    friend void *producer_fun(void *t);

    friend void *consumer_fun(void *t);

    queue consu[con_count];

    void consument() {
        size_t consumer_number;
        for (consumer_number = 0; !pthread_equal(consumers[consumer_number], pthread_self()) &&
                                  consumer_number < con_count; consumer_number++) {}
        while (true) {
            consu[consumer_number].consumer(consumer_number);
        }
    }

    void producer() {
        size_t producer_number;
        for (producer_number = 0; !pthread_equal(producers[producer_number], pthread_self()) &&
                                  producer_number < pro_count; producer_number++) {}
        while (true) {
            int x = rand() % con_count;
            int y = x;
            while (true) {
                if (consu[(size_t) x].producer((int) producer_number, (size_t) x, false)) {
                    break;
                }
                x = (++x) % con_count;
                if (x == y) {
                    consu[(size_t) x].producer(producer_number, (size_t) x, true);
                    break;
                }
            }
        }
    }
};

Prod_Cons problem;

void *producer_fun(void *t) {
    problem.producer();
    return t;
}

void *consumer_fun(void *t) {
    problem.consument();
    return t;
}

int main() {
    srand((unsigned int) time(nullptr));

    size_t i;
    for (i = 0; i < pro_count; ++i) {
        pthread_create(&producers[i], nullptr, producer_fun, nullptr);
    }
    for (i = 0; i < con_count; ++i) {
        pthread_create(&consumers[i], nullptr, consumer_fun, nullptr);
    }
    for (i = 0; i < pro_count; ++i) {
        pthread_join(producers[i], nullptr);
    }
    for (i = 0; i < con_count; ++i) {
        pthread_join(consumers[i], nullptr);
    }
    return 0;
}