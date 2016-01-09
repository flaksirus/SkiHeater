#ifdef DEBUG_ENABLED
#define DEBUG(x) {printf x;}
#else
#define DEBUG(x)
#endif

#define CAST_TYPED_MSG(msg_id, typed_msg, orig_msg) msg_id##_T *typed_msg = (msg_id##_T*)orig_msg



