/**
 * vector
 * CS 341 - Fall 2023
 */
#include "vector.h"
#include <assert.h>
#include <stdio.h>


/**
 * 'INITIAL_CAPACITY' the initial size of the dynamically.
 */
const size_t INITIAL_CAPACITY = 8;
/**
 * 'GROWTH_FACTOR' is how much the vector will grow by in automatic reallocation
 * (2 means double).
 */
const size_t GROWTH_FACTOR = 2;

struct vector {
    /* The function callback for the user to define the way they want to copy
     * elements */
    copy_constructor_type copy_constructor;

    /* The function callback for the user to define the way they want to destroy
     * elements */
    destructor_type destructor;

    /* The function callback for the user to define the way they a default
     * element to be constructed */
    default_constructor_type default_constructor;

    /* Void pointer to the beginning of an array of void pointers to arbitrary
     * data. */
    void **array;

    /**
     * The number of elements in the vector.
     * This is the number of actual objects held in the vector,
     * which is not necessarily equal to its capacity.
     */
    size_t size;

    /**
     * The size of the storage space currently allocated for the vector,
     * expressed in terms of elements.
     */
    size_t capacity;
};

/**
 * IMPLEMENTATION DETAILS
 *
 * The following is documented only in the .c file of vector,
 * since it is implementation specfic and does not concern the user:
 *
 * This vector is defined by the struct above.
 * The struct is complete as is and does not need any modifications.
 *
 * The only conditions of automatic reallocation is that
 * they should happen logarithmically compared to the growth of the size of the
 * vector inorder to achieve amortized constant time complexity for appending to
 * the vector.
 *
 * For our implementation automatic reallocation happens when -and only when-
 * adding to the vector makes its new  size surpass its current vector capacity
 * OR when the user calls on vector_reserve().
 * When this happens the new capacity will be whatever power of the
 * 'GROWTH_FACTOR' greater than or equal to the target capacity.
 * In the case when the new size exceeds the current capacity the target
 * capacity is the new size.
 * In the case when the user calls vector_reserve(n) the target capacity is 'n'
 * itself.
 * We have provided get_new_capacity() to help make this less ambigious.
 */

static size_t get_new_capacity(size_t target) {
    /**
     * This function works according to 'automatic reallocation'.
     * Start at 1 and keep multiplying by the GROWTH_FACTOR untl
     * you have exceeded or met your target capacity.
     */
    size_t new_capacity = 1;
    while (new_capacity < target) {
        new_capacity *= GROWTH_FACTOR;
    }
    return new_capacity;
}

vector *vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor,
                      default_constructor_type default_constructor) {
    // your code here
    if (copy_constructor && destructor && default_constructor) {
        vector *v = malloc(sizeof(vector));
        v->copy_constructor = copy_constructor;
        v->destructor = destructor;
        v->default_constructor = default_constructor;
        v->array = malloc(sizeof(void *) * INITIAL_CAPACITY);
        v->size = 0;
        v->capacity = INITIAL_CAPACITY;
        return v;
    } else {
        copy_constructor_type c = copy_constructor;
        destructor_type d = destructor;
        default_constructor_type dc = default_constructor;
        if (!copy_constructor) {
            c = shallow_copy_constructor;
        }
        if (!destructor) {
            d = shallow_destructor;
        }
        if (!default_constructor) {
            dc = shallow_default_constructor;
        }
        return vector_create(c, d, dc);
    }
}

void vector_destroy(vector *this) {
    assert(this);
    // your code here
    for (size_t i = 0; i < this->size; i++) {
        this->destructor(this->array[i]);
        this->array[i] = NULL;
    }
    free(this->array);
    free(this);
    this = NULL;
}

void **vector_begin(vector *this) {
    return this->array + 0;
}

void **vector_end(vector *this) {
    return this->array + this->size;
}

size_t vector_size(vector *this) {
    assert(this);
    // your code here
    return this->size;
}

void vector_resize(vector *this, size_t n) {
    assert(this);
    assert(n >= 0);
    // your code here
    size_t s = vector_size(this);
    size_t c = vector_capacity(this);
    if (n > c) {
        vector_reserve(this, n);
        for (size_t i = this->size; i < n; i++){
            this->array[i] = this->default_constructor();
        }
    } else if (n > s) {
        for (size_t i = this->size; i < n; i++){
            this->array[i] = this->default_constructor();
        }
    } else if (n <= s) {
        size_t i = n;
        while (this->array[i]) {
            this->destructor(this->array[i]);
            this->array[i] = NULL;
            i++;
        }
    }
    this->size = n;
}

size_t vector_capacity(vector *this) {
    assert(this);
    // your code here
    return this->capacity;
}

bool vector_empty(vector *this) {
    assert(this);
    // your code here
    return (vector_size(this) == 0);
}

void vector_reserve(vector *this, size_t n) {
    assert(this);
    assert(n >= 0);
    // your code here
    if (n > this->capacity) {
        this->capacity = get_new_capacity(n);
        this->array = realloc(this->array, this->capacity * sizeof(void*));
    }
}

void **vector_at(vector *this, size_t position) {
    assert(this);
    assert(position >= 0);
    assert(position < this->size);
    // your code here
    return this->array + position;
}

void vector_set(vector *this, size_t position, void *element) {
    assert(this);
    assert(position >= 0);
    assert(position < vector_size(this));
    // your code here
    this->destructor(this->array[position]);
    this->array[position] = this->copy_constructor(element);
}

void *vector_get(vector *this, size_t position) {
    assert(this);
    assert(position < vector_size(this));
    assert(position >= 0);
    // your code here
    return this->array[position];
}

void **vector_front(vector *this) {
    assert(this);
    // your code here
    return this->array;
}

void **vector_back(vector *this) {
    // your code here
    return this->array + this->size - 1;
}

void vector_push_back(vector *this, void *element) {
    assert(this);
    // your code here
    vector_insert(this, this->size, element);
}

void vector_pop_back(vector *this) {
    assert(this);
    // your code here
    vector_erase(this, this->size - 1);
}

void vector_insert(vector *this, size_t position, void *element) {
    assert(this);
    // your code here
    if (position <= this->size) {
        vector_resize(this, this->size + 1);
        for (size_t i = this->size; i > position; i--) {
            this->array[i] = this->array[i-1];
        }
        if (element) {
            this->array[position] =(*this->copy_constructor)(element);
        } else {
            this->array[position] = NULL;
        }
    } else {
        vector_resize(this, position + 1);
        if (element) {
            this->array[position] = (*this->copy_constructor)(element);
        }
    }
}

void vector_erase(vector *this, size_t position) {
    assert(this);
    assert(position < vector_size(this));
    // your code here
    if (this->array[position]) {
        this->destructor(this->array[position]);
        this->array[position] = NULL;
    }
    for (size_t i = position; i < (vector_size(this) - 1); i++ ) {
        this->array[i] = this->array[i + 1];
    }
    this->array[(vector_size(this) -1)] = NULL;
    vector_resize(this, vector_size(this) - 1);

}

void vector_clear(vector *this) {
    // your code here
    vector_resize(this, 0);
}

// The following is code generated:
vector *shallow_vector_create() {
    return vector_create(shallow_copy_constructor, shallow_destructor,
                         shallow_default_constructor);
}
vector *string_vector_create() {
    return vector_create(string_copy_constructor, string_destructor,
                         string_default_constructor);
}
vector *char_vector_create() {
    return vector_create(char_copy_constructor, char_destructor,
                         char_default_constructor);
}
vector *double_vector_create() {
    return vector_create(double_copy_constructor, double_destructor,
                         double_default_constructor);
}
vector *float_vector_create() {
    return vector_create(float_copy_constructor, float_destructor,
                         float_default_constructor);
}
vector *int_vector_create() {
    return vector_create(int_copy_constructor, int_destructor,
                         int_default_constructor);
}
vector *long_vector_create() {
    return vector_create(long_copy_constructor, long_destructor,
                         long_default_constructor);
}
vector *short_vector_create() {
    return vector_create(short_copy_constructor, short_destructor,
                         short_default_constructor);
}
vector *unsigned_char_vector_create() {
    return vector_create(unsigned_char_copy_constructor,
                         unsigned_char_destructor,
                         unsigned_char_default_constructor);
}
vector *unsigned_int_vector_create() {
    return vector_create(unsigned_int_copy_constructor, unsigned_int_destructor,
                         unsigned_int_default_constructor);
}
vector *unsigned_long_vector_create() {
    return vector_create(unsigned_long_copy_constructor,
                         unsigned_long_destructor,
                         unsigned_long_default_constructor);
}
vector *unsigned_short_vector_create() {
    return vector_create(unsigned_short_copy_constructor,
                         unsigned_short_destructor,
                         unsigned_short_default_constructor);
}
