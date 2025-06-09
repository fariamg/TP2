//@ Enum que define os estados possíveis de um pacote
enum PackageState {
    NOT_POSTED,              
    SCHEDULED_ARRIVAL,      
    ARRIVED_NOT_STORED,      
    STORED,                  
    ALLOCATED_FOR_TRANSPORT, 
    DELIVERED,              
};