#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define green_light_counter 15
#define red_light_counter 10
typedef struct Car
{
    int identifier;
    int speed;
    int time;
    struct Car *next;
} car;

typedef struct Queue
{
    car *front;
    car *rear;
} queue;

// traffic accumulating (insert at tail)
void enqueue(queue *q, car *c)
{
    c->next = NULL;

    if (q->front == NULL)
    {
        q->front = c;
    }
    else
    {
        q->rear->next = c;
    }

    q->rear = c;
}

// traffic releasing (remove from front)
void dequeue(queue *q)
{

    if (q->front == NULL)
    {
        printf("There is no traffic\n");
    }
    else
    {
        car *temp = q->front;
        q->front = temp->next;
        free(temp);
    }
}

// Checking car ID at front of queue
car *peek(queue *q)
{
    if (q->front == NULL)
    {
        return NULL;
    }

    return q->front;
}

// Displaying the queue using car IDs
void display(queue *q)
{
    car *c = q->front;
    while (c != NULL)
    {
        printf("%d->", c->identifier);
        c = c->next;
    }
    printf("END\n");
}

int main()
{
    // generating a random number of cars at the start of the program
    srand(time(NULL));
    int random_number = rand() % 10 + 1; // cars from 1 through 10
    printf("Number of cars at the traffic signal are: %d\n", random_number);

    // declaring the structures
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = NULL;
    q->rear = NULL;
    car *prev=q->front;

    // adding initial 'random_number' cars to the queue and assigning random speeds & time to reach traffic signal
    printf("ID\t S\t T\t D\t\n");
    for (int i = 1; i <= random_number; i++)
    {
        car *c = (car *)malloc(sizeof(car));
        c->identifier = i;
        c->speed = rand() % 10 + 1;
        c->time = rand() % 20 + 1;
        if (prev != NULL)
        {
            c->time+=prev->time;
        }
        prev = c;
        printf("%d\t %d\t %d\t %.2f\n", c->identifier, c->speed, c->time, (double)c->time/(double)c->speed);
        enqueue(q, c);
    }
    display(q);

    int green_light_time = green_light_counter;
    int red_light_time = red_light_counter;
    int total_time = 0;      //count
    while (1)
    {
        car *car = peek(q);
        if (car != NULL)
        {
            // case 1: time <= 0
            if (car->time <= 0 && green_light_time>0)
            {
                // printf("%d\n", green_light_time);
                printf("Car %d passed the signal.\n", car->identifier);
                dequeue(q);
                red_light_time = 10;
                
            }
            else if (red_light_time!=0 && green_light_time <=0)
            {
                printf("Red traffic light. STOP!\n");
                green_light_time = green_light_counter;
                total_time+=red_light_counter;
                while (car != NULL)
                {
                    car->time -= red_light_counter;
                    car=car->next;
                }
                for (int i = red_light_counter; i >=0 ; i--)
                {
                    printf("%d\n", i);
                }
                printf("Green light. GO!\n");
            }
            else
            {
                printf("\tCar %d is arriving at the traffic light (%.2f meters remaining).\n", car->identifier, (double)car->time/(double)car->speed);
                car->time -= car->speed;
                total_time++;
                green_light_time--;
            }
        }
        else
        {
            printf("No cars at traffic signal.\n");
            break;
        }

        if (green_light_time <=0 ) 
        {
            red_light_time--;
        }
    }
    // printf("Total waiting period for all the cars is %d secs.\n", total_time);
    return 0;
}