/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library
    http://spirit.sourceforge.net/

    Copyright (c) 2001 Daniel C. Nuffer.
    Copyright (c) 2001-2005 Hartmut Kaiser. 
    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <cstdlib>
#include <cstring>

#include <boost/assert.hpp>

#include <boost/wave/wave_config.hpp>          // configuration data
#include <boost/wave/cpplexer/re2clex/aq.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace cpplexer {
namespace re2clex {

int aq_grow(aq_queue q)
{
    std::size_t new_size = q->max_size << 1;
    aq_stdelement* new_queue = (aq_stdelement*)realloc(q->queue,
            new_size * sizeof(aq_stdelement));

    BOOST_ASSERT(q);
    BOOST_ASSERT(q->max_size < 100000);
    BOOST_ASSERT(q->size <= q->max_size);

#define ASSERT_SIZE BOOST_ASSERT( \
    ((q->tail + q->max_size + 1) - q->head) % q->max_size == \
    q->size % q->max_size)

    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    if (!new_queue)
    {
        BOOST_ASSERT(0);
        return 0;
    }

    q->queue = new_queue;
    if (q->tail <= q->head) /* tail has wrapped around */
    {
        /* move the tail from the beginning to the end */
        memcpy(q->queue + q->max_size, q->queue,
                (q->tail + 1) * sizeof(aq_stdelement));
        q->tail += q->max_size;
    }
    q->max_size = new_size;

    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    return 1;
}

int aq_enqueue(aq_queue q, aq_stdelement e)
{
    BOOST_ASSERT(q);
    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);


    if (AQ_FULL(q))
        if (!aq_grow(q))
            return 0;

    ++q->tail;
    if (q->tail == q->max_size)
        q->tail = 0;

    q->queue[q->tail] = e;
    ++q->size;

    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    return 1;
}

int aq_enqueue_front(aq_queue q, aq_stdelement e)
{
    BOOST_ASSERT(q);

    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);


    if (AQ_FULL(q))
        if (!aq_grow(q))
            return 0;

    if (q->head == 0)
        q->head = q->max_size - 1;
    else
        --q->head;

    q->queue[q->head] = e;
    ++q->size;

    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    return 1;
}

int aq_serve(aq_queue q, aq_stdelement *e)
{

    BOOST_ASSERT(q);
    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);


    if (AQ_EMPTY(q))
        return 0;

    *e = q->queue[q->head];
    return aq_pop(q);
}

int aq_pop(aq_queue q)
{

    BOOST_ASSERT(q);
    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);


    if (AQ_EMPTY(q))
        return 0;

    ++q->head;
    if (q->head == q->max_size)
        q->head = 0;
    --q->size;

    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    return 1;
}

aq_queue aq_create(void)
{
    aq_queue q;

    q = (aq_queue)malloc(sizeof(aq_queuetype));
    if (!q)
    {
        return 0;
    }

    q->max_size = 8; /* initial size */
    q->queue = (aq_stdelement*)malloc(
            sizeof(aq_stdelement) * q->max_size);
    if (!q->queue)
    {
        free(q);
        return 0;
    }

    q->head = 0;
    q->tail = q->max_size - 1;
    q->size = 0;


    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    return q;
}

void aq_terminate(aq_queue q)
{

    BOOST_ASSERT(q);
    BOOST_ASSERT(q->size <= q->max_size);
    ASSERT_SIZE;
    BOOST_ASSERT(q->head <= q->max_size);
    BOOST_ASSERT(q->tail <= q->max_size);

    free(q->queue);
    free(q);
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace re2clex
}   // namespace cpplexer
}   // namespace wave
}   // namespace boost
