#ifndef BYTECOIN_IO_H
#define BYTECOIN_IO_H

#include <stdint.h>
#include "bytecoin_crypto.h"

#pragma pack(push)
#pragma pack(1)
typedef struct io_call_params_s
{
    uint8_t cla;
    uint8_t ins;
    uint8_t p1;
    uint8_t p2;
    uint8_t lc;
//    uint8_t le;
} io_call_params_t;

typedef struct io_call_s
{
    io_call_params_t params;
    uint8_t cdata[0];
} io_call_t;
#pragma pack(pop)

void init_io_call_params(io_call_params_t* ioparams);

//#define BYTECOIN_IO_BUFFER_SIZE 288
//#define BYTECOIN_IO_BUFFER_SIZE 234

#define BYTECOIN_IO_BUFFER_SIZE 163
#if BYTECOIN_IO_BUFFER_SIZE < 163 // 5*32+1 bytes in export_view_only +2 bytes for sw
#error BYTECOIN_IO_BUFFER_SIZE is too small
#endif

typedef struct confirm_tx_params_s
{
    public_key_t address_s;
    public_key_t address_sv;
    uint64_t amount;
    uint64_t fee;
    uint8_t address_tag;
}  confirm_tx_params_t;

typedef struct io_buffer_s
{
    union
    {
        uint8_t data[BYTECOIN_IO_BUFFER_SIZE];
        confirm_tx_params_t confirm_tx_params;
    };
    uint16_t length;
    uint16_t offset;
} io_buffer_t;

void init_io_buffer(io_buffer_t* iobuf);

void print_io_call_params(const io_call_params_t* iocall_params);
void print_io_call(const io_call_t* iocall);
void print_io_buffer(const io_buffer_t* iobuf);

void reset_io_buffer(io_buffer_t* iobuf);
void clear_io_buffer(io_buffer_t* iobuf);
int io_do(io_call_params_t* previous_iocall_params, io_buffer_t* iobuf, uint32_t io_flags);

uint64_t fetch_var_from_io_buffer(io_buffer_t* iobuf, uint16_t len);
//void fetch_reversed_bytes_from_io_buffer(io_buffer_t* iobuf, void* buf, uint16_t len);
void fetch_bytes_from_io_buffer(io_buffer_t* iobuf, void* buf, uint16_t len);
elliptic_curve_point_t fetch_point_from_io_buffer(io_buffer_t* iobuf);
elliptic_curve_scalar_t fetch_scalar_from_io_buffer(io_buffer_t* iobuf);
hash_t fetch_hash_from_io_buffer(io_buffer_t* iobuf);

//void insert_reversed_bytes_to_io_buffer(io_buffer_t* iobuf, const void* buf, uint16_t len);
//void insert_bytes_to_io_buffer(io_buffer_t* iobuf, const void* buf, uint16_t len);
void insert_var_to_io_buffer(io_buffer_t* iobuf, uint64_t var, uint16_t len);
void insert_scalar_to_io_buffer(io_buffer_t* iobuf, const elliptic_curve_scalar_t* s);
void insert_point_to_io_buffer(io_buffer_t* iobuf, const elliptic_curve_point_t* P);
void insert_hash_to_io_buffer(io_buffer_t* iobuf, const hash_t* h);

//inline
//void insert_scalar_to_io_buffer(io_buffer_t* iobuf, const elliptic_curve_scalar_t* s)
//{
//    insert_reversed_bytes_to_io_buffer(iobuf, s->data, sizeof(s->data));
//}

//inline
//void insert_point_to_io_buffer(io_buffer_t* iobuf, const elliptic_curve_point_t* P)
//{
//    insert_bytes_to_io_buffer(iobuf, P->data, sizeof(P->data));
//}

//inline
//void insert_hash_to_io_buffer(io_buffer_t* iobuf, const hash_t* h)
//{
//    insert_bytes_to_io_buffer(iobuf, h->data, sizeof(h->data));
//}

#define insert_point(primitive) \
    insert_point_to_io_buffer(&G_bytecoin_vstate.io_buffer, &primitive)

#define insert_scalar(primitive) \
    insert_scalar_to_io_buffer(&G_bytecoin_vstate.io_buffer, &primitive)

#define insert_hash(primitive) \
    insert_hash_to_io_buffer(&G_bytecoin_vstate.io_buffer, &primitive)

#define fetch_point() \
    fetch_point_from_io_buffer(&G_bytecoin_vstate.io_buffer)

#define fetch_scalar() \
    fetch_scalar_from_io_buffer(&G_bytecoin_vstate.io_buffer)

#define fetch_hash() \
    fetch_hash_from_io_buffer(&G_bytecoin_vstate.io_buffer)

#define fetch_var(var_or_type) \
    fetch_var_from_io_buffer(&G_bytecoin_vstate.io_buffer, sizeof(var_or_type))

#define insert_var(var) \
    insert_var_to_io_buffer(&G_bytecoin_vstate.io_buffer, var, sizeof(var))

#define fetch_secret_key  fetch_scalar
#define fetch_public_key  fetch_point
#define fetch_keyimage    fetch_point
#define insert_secret_key insert_scalar
#define insert_public_key insert_point
#define insert_keyimage   insert_point


#endif // BYTECOIN_IO_H