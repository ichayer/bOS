#include <semaphores.h>
#include <graphics.h>

// A list might be implemented to track de asociated processes to a semaphore
// Now just are being counted the number of processes, but this could lead to an inconsistency if,
// for example, a process tries to close the same semaphore twice
typedef struct {
    Tsem semId; 
    uint8_t value;
    //Tlock lock;
    uint8_t linkedProcesses; 
    char* name;
    //TWaitQueue lockedProcesses;   // Tracks the processes waiting for the lock  
    TWaitQueue waitingProcesses;  //Tracks the processes waiting for a post in this semaphore
} TSemaphore;

TSemaphore * semaphores[MAX_SEMAPHORES] = {NULL};
TResourceNamer namer;
/*Tlock lock;


extern int _lock(Tlock * lock);

static void tryLockLocalSem(sem){
    while(_lock(&(semaphores[sem]->lock))!=0){
        // bloquear el proceso
        // yield 
    }
}

static void unlock(sem){
    semaphores[sem]->lock = 0;
    // Si hay lockedProcesses en la wq, desbloquearlos
}*/

static int sem_free(Tsem sem) {
    int value = rnm_unnameResource(namer, semaphores[sem]->name); 
    value += wq_free(semaphores[sem]->waitingProcesses);
    value += mm_free(semaphores[sem]);
    semaphores[sem] = NULL;
    if(value!=0)
        return SEM_FAILED;
    return SEM_SUCCES;
}

static int isValidSemId(Tsem sem) {
    return sem>=0 && sem<MAX_SEMAPHORES && semaphores[sem]!=NULL;
}

int sem_printDebug() {

    for(int sem = 0 ; sem < MAX_SEMAPHORES ; ++sem) {
        if(semaphores[sem]!=NULL){
            scr_print("Id: ");
            scr_printDec(sem);
            scr_print(", \t Value: ");
            scr_printDec(semaphores[sem]->value);
            scr_print(", \t Linked processes: ");
            scr_printDec(semaphores[sem]->linkedProcesses);
            scr_print(", \t Name: ");
            scr_print(semaphores[sem]->name);
            scr_printChar('\n');
        }

    }
}

int sem_init() {
    namer = rnm_new();
    //lock = 0;
    if(namer != 0)
        return SEM_FAILED;
    return 0;
}

Tsem sem_open(const char * name, uint8_t initialValue) {

    //while(_lock(&lock)!=0);

    TSemaphore * sem = rnm_getResource(namer, name);

    if(sem!=NULL){
        sem->linkedProcesses+=1;
        //lock = 0;
        return sem->semId;
    }
    
    int i;
    for( i=0 ; i < MAX_SEMAPHORES && semaphores[i]; ++i );
    if( i == MAX_SEMAPHORES )
        return SEM_FAILED;
    
    semaphores[i] = mm_malloc(sizeof(TSemaphore));
    if(semaphores[i] == NULL){
        return SEM_FAILED;
    }
    semaphores[i]->value = initialValue;
    semaphores[i]->semId = i;
    //semaphores[i]->lock = 0;
    semaphores[i]->linkedProcesses = 1;
    semaphores[i]->waitingProcesses = wq_new();
    if(semaphores[i]->waitingProcesses == NULL){
        mm_free(semaphores[i]);
        return SEM_FAILED;
    }

    if(rnm_nameResource(namer, semaphores[i], name, &(semaphores[i]->name))!=0){
        //check what should be freed
        return SEM_FAILED;
    }
    //lock = 0;
    return (Tsem) i;
}

int sem_close(Tsem sem) {
    if(!isValidSemId(sem) || semaphores[sem] == NULL)
        return SEM_NOTEXISTS;

    if(semaphores[sem]->linkedProcesses == 1)
        return sem_free(sem);                    
    
    semaphores[sem]->linkedProcesses-= 1;
    return SEM_SUCCES;
}

int sem_post(Tsem sem) {
    if(!isValidSemId(sem))
        return SEM_NOTEXISTS;
    
    if(semaphores[sem]->value > 0 || wq_unblockSingle(semaphores[sem]->waitingProcesses) != 0){
        semaphores[sem]->value+=1;
    }
    return SEM_SUCCES;
}

int sem_wait(Tsem sem) {
    if(!isValidSemId(sem))
        return SEM_NOTEXISTS;
    
    if(semaphores[sem]->value > 0){
        semaphores[sem]->value-=1;
        return SEM_SUCCES;
    }

    TPid cpid = sch_getCurrentPID();

    scr_printDec(cpid);
    scr_printDec(1);
    if((wq_addIfNotExists(semaphores[sem]->waitingProcesses, cpid ))==0) {
        scr_printDec(cpid);
        scr_printDec(2);
        sch_blockProcess(cpid);
        sch_yieldProcess();
    }

    return SEM_SUCCES;
}