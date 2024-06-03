#define DEF_KEY_FILE "key"
#define SIZE -256

typedef struct {  
    long type;  
    char buf[100];
} Message;
// первое число - номер семафора
// второе число - число операции:
//      - положительное число - оно прибавляется к счетчику семафора
//      - отрицательное число - ожидание пока значение счетчика не станет равным или больше
//                              модуля этого числа, затем из счетчика вычитается модуль этого числа
// третье число - флаги
static struct sembuf waitNotFull[1] = {256,SIZE,0};
static struct sembuf releaseFull[1] = {256,1,0};

static struct sembuf mem_lock[1] = {0,256,0};
static struct sembuf mem_unlock[1] = {256,SIZE,0};

static struct sembuf waitNotEmpty[1] = {0,SIZE,0};
static struct sembuf releaseEmpty[1] = {0,1,0};
static struct sembuf setFree[1] = {256,SIZE,0};