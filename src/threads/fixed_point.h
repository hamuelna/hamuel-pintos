#define NPOINT (1<<14)

// convert int to fix
#define CONVERT_TO_FIX(a) (a*NPOINT)

//convert fix to ine
#define ROUND_TO_ZERO(a) (a/NPOINT)
#define ROUND_TO_NEAREST(a) (a >= 0 ? ((a + (NPOINT/2)) / NPOINT) \
								    : ((a - (NPOINT/2)) / NPOINT) )

#define ADD(a,b) (a+b)
#define ADD_MIXED(a,n) (a + (n*NPOINT))

#define SUBT(a,b) (a-b)
#define SUBT_MIXED(a,n) (a - (n*NPOINT))

#define MULT(a,b) (((int64_t)a)*b)/NPOINT
#define MULT_MIXED(a,n) (a*n)

#define DIV(a,b) ((((int64_t) a)*NPOINT)/b)
#define DIV_MIXED(a,n) (a/n)



// // convert int to fix
// #define CONVERT_TO_FIX(a) ((a)<<NPOINT)

// //convert fix to ine
// #define ROUND_TO_ZERO(a) ((a)>>NPOINT)
// #define ROUND_TO_NEAREST(a) (a >= 0 ? ((a + (NPOINT>>2)) >> NPOINT) \
// 								    : ((a - (NPOINT>>2)) >> NPOINT) )

// #define ADD(a,b) (a+b)
// #define ADD_MIXED(a,n) (a + (n<<NPOINT))

// #define SUBT(a,b) (a-b)
// #define SUBT(a,n) (a - (n<<NPOINT))

// #define MULT(a,b) (((int64_t)a)*b)>>NPOINT
// #define MULT_MIXED(a,n) (a*n)

// #define DIV(a,b) ((((int64_t) a)<<NPOINT)/b)
// #define DIV_MIXED(a,n) (a/n)

