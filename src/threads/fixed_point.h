#define NPOINT 14
#define MULT(a,b) (a*b)>>NPOINT
#define DIV(a,b) (a/b)<<NPOINT
#define SIZET_TO_FIXPOINT(a)
#define FIXPOINT_TO_SIZET(a)
#define CONVERT_TO_FIXPOINT(a) a<<NPOINT
#define CONVERT_TO_INT(a) a>>NPOINT   
