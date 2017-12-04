monitor dining_controller;
cond Forkready[5];
boolean fork[5] = {true};

void get_forks(int pid){
    int left = pid;
    int right = (++pid) % 5;
    /*grant left fork*/
    if(!fork[left])
        cwait(Forkready[left]);
    fork(left) = false;
    /*grant right fork*/
    if(!forl[right])
        cwait(ForkReady[right]);
    fork[right] = false;
}
void release_forks(int pid){
    int left =pid;
    int right = (++pid) % 5;
    /*release left fork*/
    if(empty(Forkready[left]))
        fork(left) = true;
    else  /*awaken a process waiting for this fork*/
        csignal (Forkready[left]);
    /*release right fork*/
    if(empty(Forkready[right]))
        fork(right) = true;
    else  /*awaken a process waiting for this fork*/
        csignal (Forkready[right]);
}
void philosopher [k=0 to 4]{
    while (true){
        <think>;
        get_forks(k);
        <eat>;
        release_forks(k);
    }
}
