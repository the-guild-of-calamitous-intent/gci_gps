#include "ubx_parser.h"
#include <stdlib.h>
#include <string.h>

// Initialize the parser
void ubx_parser_init(ubx_parser_t *parser) {
  parser->state                  = UBX_STATE_SYNC1;
  parser->message.msg_class      = 0;
  parser->message.msg_id         = 0;
  parser->message.payload_length = 0;
  parser->message.payload        = NULL;
  parser->message.ck_a           = 0;
  parser->message.ck_b           = 0;
  parser->payload_bytes_received = 0;
  parser->ck_a                   = 0;
  parser->ck_b                   = 0;
}

// Process a single byte and update parser state
bool ubx_parser_process_byte(ubx_parser_t *parser, uint8_t byte, ubx_message_t *msg) {
  switch (parser->state) {
  case UBX_STATE_SYNC1:
    if (byte == UBX_SYNC1) {
      parser->state = UBX_STATE_SYNC2;
    }
    break;

  case UBX_STATE_SYNC2:
    if (byte == UBX_SYNC2) {
      parser->state = UBX_STATE_CLASS;
      parser->ck_a  = 0;
      parser->ck_b  = 0;
    }
    else {
      parser->state = UBX_STATE_SYNC1;
    }
    break;

  case UBX_STATE_CLASS:
    parser->message.msg_class = byte;
    parser->ck_a += byte;
    parser->ck_b += parser->ck_a;
    parser->state = UBX_STATE_ID;
    break;

  case UBX_STATE_ID:
    parser->message.msg_id = byte;
    parser->ck_a += byte;
    parser->ck_b += parser->ck_a;
    parser->state = UBX_STATE_LENGTH1;
    break;

  case UBX_STATE_LENGTH1:
    parser->message.payload_length = byte;
    parser->ck_a += byte;
    parser->ck_b += parser->ck_a;
    parser->state = UBX_STATE_LENGTH2;
    break;

  case UBX_STATE_LENGTH2:
    parser->message.payload_length |= (byte << 8);
    parser->ck_a += byte;
    parser->ck_b += parser->ck_a;
    parser->state                  = UBX_STATE_PAYLOAD;
    parser->payload_bytes_received = 0;
    // Allocate memory for payload
    parser->message.payload = (uint8_t *)malloc(parser->message.payload_length);
    if (!parser->message.payload) {
      parser->state = UBX_STATE_SYNC1;
      return false; // Memory allocation failed
    }
    break;

  case UBX_STATE_PAYLOAD:
    if (parser->payload_bytes_received < parser->message.payload_length) {
      parser->message.payload[parser->payload_bytes_received++] = byte;
      parser->ck_a += byte;
      parser->ck_b += parser->ck_a;
    }
    if (parser->payload_bytes_received == parser->message.payload_length) {
      parser->state = UBX_STATE_CK_A;
    }
    break;

  case UBX_STATE_CK_A:
    parser->message.ck_a = byte;
    parser->state        = UBX_STATE_CK_B;
    break;

  case UBX_STATE_CK_B:
    parser->message.ck_b = byte;
    parser->state        = UBX_STATE_SYNC1;
    // Copy message to output
    *msg = parser->message;
    // Validate checksum
    if (ubx_validate_checksum(msg)) {
      return true; // Valid message received
    }
    else {
      free(msg->payload); // Free payload on invalid checksum
      msg->payload = NULL;
      return false;
    }
    break;

  default:
    parser->state = UBX_STATE_SYNC1;
    break;
  }
  return false; // Message not complete
}

// Validate checksum
bool ubx_validate_checksum(const ubx_message_t *msg) {
  uint8_t ck_a = 0, ck_b = 0;
  ck_a += msg->msg_class;
  ck_b += ck_a;
  ck_a += msg->msg_id;
  ck_b += ck_a;
  ck_a += (msg->payload_length & 0xFF);
  ck_b += ck_a;
  ck_a += (msg->payload_length >> 8);
  ck_b += ck_a;
  for (uint16_t i = 0; i < msg->payload_length; i++) {
    ck_a += msg->payload[i];
    ck_b += ck_a;
  }
  return (ck_a == msg->ck_a && ck_b == msg->ck_b);
}

// Parse NAV-PVT message
bool ubx_parse_nav_pvt(const ubx_message_t *msg, ubx_nav_pvt_t *nav_pvt) {
  if (msg->msg_class != UBX_NAV_PVT_CLASS || msg->msg_id != UBX_NAV_PVT_ID ||
      msg->payload_length != UBX_NAV_PVT_LENGTH) {
    return false;
  }

  // Extract fields from payload (little-endian)
  uint8_t *p       = msg->payload;
  nav_pvt->iTOW    = *(uint32_t *)(p + 0);
  nav_pvt->year    = *(uint16_t *)(p + 4);
  nav_pvt->month   = p[6];
  nav_pvt->day     = p[7];
  nav_pvt->hour    = p[8];
  nav_pvt->min     = p[9];
  nav_pvt->sec     = p[10];
  nav_pvt->valid   = p[11];
  nav_pvt->tAcc    = *(uint32_t *)(p + 12);
  nav_pvt->nano    = *(int32_t *)(p + 16);
  nav_pvt->fixType = p[20];
  nav_pvt->flags   = p[21];
  nav_pvt->flags2  = p[22];
  nav_pvt->numSV   = p[23];
  nav_pvt->lon     = *(int32_t *)(p + 24);
  nav_pvt->lat     = *(int32_t *)(p + 28);
  nav_pvt->height  = *(int32_t *)(p + 32);
  nav_pvt->hMSL    = *(int32_t *)(p + 36);
  nav_pvt->hAcc    = *(uint32_t *)(p + 40);
  nav_pvt->vAcc    = *(uint32_t *)(p + 44);
  nav_pvt->velN    = *(int32_t *)(p + 48);
  nav_pvt->velE    = *(int32_t *)(p + 52);
  nav_pvt->velD    = *(int32_t *)(p + 56);
  nav_pvt->gSpeed  = *(int32_t *)(p + 60);
  nav_pvt->headMot = *(int32_t *)(p + 64);
  nav_pvt->sAcc    = *(uint32_t *)(p + 68);
  nav_pvt->headAcc = *(uint32_t *)(p + 72);
  nav_pvt->pDOP    = *(uint16_t *)(p + 76);
  memcpy(nav_pvt->reserved1, p + 78, 6);
  nav_pvt->headVeh = *(int32_t *)(p + 84);
  nav_pvt->magDec  = *(int16_t *)(p + 88);
  nav_pvt->magAcc  = *(uint16_t *)(p + 90);

  return true;
}