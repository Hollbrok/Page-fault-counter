 
// CPP program to demonstrate optimal page
// replacement algorithm.
#include <bits/stdc++.h>
#include <queue>
#include <deque>
#include <algorithm>

template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};

long getNumber(char *numString);
bool search(int key, std::vector<int>& fr);
int predict(int pg[], std::vector<int>& fr, int pn, int index);
void optimalPage(int pg[], int pn, int fn);
int pageFaultsFIFO(int pages[], int n, int capacity);
void pageFaults(int* ref, int len, int frame_size, int ctime);

#define PAGES { 1, 2, 3, 5, 2, 1, 4, 5, 2, 1, 3, 4, 1, 2, 5, 1, 3, 2, 4 }

int main(int argc, char* argv[])
{
    if (argc > 2)
    {   
        //fprintf(stderr, "arguments are provided (need number of available pages + space separated page numbers)\n");
        int pn = argc - 2;
        int pg[pn];//= PAGES;
        
        for (int i = 0; i < pn; ++i)
            pg[i] = getNumber(argv[2 + i]);

        int fn = getNumber(argv[1]);

        std::cout << "\t\t OPT (optimal):" << std::endl;
        optimalPage(pg, pn, fn);

        std::cout << "\n\t\t FIFO (Fist Input Fist Output):" << std::endl;

        //int pages[] = PAGES;
        //int n = sizeof(pages)/sizeof(pages[0]);
        //int capacity = 3;

        pageFaultsFIFO(pg, pn, fn);

        std::cout << "\n\t\t LRU (the Least Recently Used):" << std::endl;

        //(int fn, int* pg, int len, int ctime)
        //(int* ref, int len, int frame_size, int ctime)
        
        pageFaults(pg, pn, fn, 0);

    }
    else
    {
        fprintf(stderr, "no arguments are provided (need number of available pages + space separated page numbers)\n"
                        "processing with default set of pages\n");

        int pg[] = PAGES;
        int pn = sizeof(pg) / sizeof(pg[0]);
        int fn = 3;
        optimalPage(pg, pn, fn);

    }

    return 0;
}

long getNumber(char *numString)
{
    if (numString == NULL)
    {
        fprintf(stderr, "null string argument\n");
        exit(EXIT_FAILURE);
    }
    if (*numString == '\0')
    {
        fprintf(stderr, "empty number argument\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;

    long gNumber;
    char* endOfEnter;

    const int baseOfNumber = 10;
    gNumber = strtol(numString, &endOfEnter, baseOfNumber);

    if(*endOfEnter != '\0')
    {
        fprintf(stderr, "strtol error\n");
        exit(EXIT_FAILURE);
    }
    if (errno != 0)
    {
        fprintf(stderr, "strtol error\n");
        exit(EXIT_FAILURE);
    }
    
    return gNumber;

}
  
// Function to check whether a page exists
// in a frame or not
bool search(int key, std::vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}
  
// Function to find the frame that will not be used
// recently in future after given index in pg[0..pn-1]
int predict(int pg[], std::vector<int>& fr, int pn, int index)
{
    // Store the index of pages which are going
    // to be used recently in future
    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
  
        // If a page is never referenced in future,
        // return it.
        if (j == pn)
            return i;
    }
  
    // If all of the frames were not in future,
    // return any of them, we return 0. Otherwise
    // we return res.
    return (res == -1) ? 0 : res;
}
  
void optimalPage(int pg[], int pn, int fn)
{
    // Create an array for given number of
    // frames and initialize it as empty.
    std::vector<int> fr;

    std::size_t arr_size = pn;
    std::vector<int> *table = new std::vector<int>[fn + 1]{};
  
    // Traverse through page reference array
    // and check for miss and hit.
    int hit = 0;
    for (int i = 0; i < pn; i++) {
  
        // Page found in a frame : HIT
        if (search(pg[i], fr)) {
            hit++;
            for (int j = 0; j < fn; ++j)
                table[j].push_back(fr[j]);
            table[fn].push_back(0);
            continue;
        }
  
        // Page not found in a frame : MISS
  
        // If there is space available in frames.
        if (fr.size() < fn)
            fr.push_back(pg[i]);
  
        // Find the page to be replaced.
        else {
            int j = predict(pg, fr, pn, i + 1);
            fr[j] = pg[i];
        }


        /* заполняем таблицу */
        for (int j = 0; j < fn; ++j)
            table[j].push_back(fr[j]);

        table[fn].push_back(1);

    }
    
    //std::cout << "No. of hits = " << hit << std::endl;
    std::cout << "No. of PF = " << pn - hit << "\n\t";

    for (int i = 0; i < fn; ++i)
    {
        for (int j = 0; j < pn; ++j)
            std::cout << table[i][j] << " ";
        std::cout << "\n\t";
    }

    for (int j = 0; j < pn; ++j)
        if (table[fn][j] == 0) 
            std::cout << "- ";
        else
            std::cout << "+ ";

    std::cout << "\n";
}
  
// Function to find page faults using FIFO

int pageFaultsFIFO(int pages[], int n, int capacity)
{
    int i,j,frame[capacity],k,avail,count=0;

    for (i = 0; i < capacity; i++)
        frame[i]= -1;

    j = 0;
    printf("P:     \t\t      page frames\t\tPF status\n");

    for (i = 0; i < n; i++)
    {
        printf("%3d\t\t",pages[i]);
        avail = 0;
        for (k = 0; k < capacity; k++)
            if (frame[k] == pages[i])
                avail = 1;

        if (avail == 0)
        {
            frame[j] = pages[i];
            j = (j + 1) % capacity;
            count++;
        }

        for (k = 0; k < capacity; k++)
            printf("%3d\t",frame[k]);
        
        if (avail == 0) // no PF
        {
            printf("\t+");
        }
        else
            printf("\t-");

        printf("\n");
    }
    
    std::cout << "No. of PF = " << count << "\n\t";
    
    return 0;
}

//////////////////////////

void pageFaults(int* ref, int len, int frame_size, int ctime)
{
    // frame_size = 3
    // len = 19


    // Arr to simulate frames

    std::vector<int> *table = new std::vector<int>[frame_size + 1]{};

  
    int cnt = 0;
    // Arr to simulate frames
    std::vector<std::pair<int, int>> arr;
  
    // To initialise the array
    for (int i = 0; i < frame_size; i++) {
        arr.push_back(std::make_pair(0, ctime));
    }
  
    int page;

    int totalFilled = 0;
  
    for (int i = 0; i < len; i++) {
        page = ref[i];
        auto it = arr.begin();
  
        for (it = arr.begin(); it != arr.end(); it++) {
            if (it->first == page) {
                break;
            }
        }
  
        // If page is found
        if (it != arr.end()) {
            // update the value of
            // current time
            it->second = ctime;

            int j = 0;
            for (auto iter = arr.begin(); iter != arr.end(); iter++)
                table[j++].push_back(iter->first);

            table[frame_size].push_back(0);
        }
  
        // If page is not found
        else {
            // Find the page with min value of ctime,
            // as it will be the least recently used

            std::vector<std::pair<int, int> >::iterator pos;
            pos = arr.begin();
            int min = pos->second;
            
            for (auto itr = arr.begin(); itr != arr.end(); itr++) 
            {
                if (itr->second < min) 
                {
                    pos = itr;
                    min = pos->second;
                }
            }
  
            // Erase this page from the frame vector
            arr.erase(pos);
  
            if (totalFilled > frame_size - 1)
            {
                //std::cout << "page = " << page << std::endl;
                arr.insert(pos, std::make_pair(page, ctime));
                int j = 0;
                for (auto iter = arr.begin(); iter != arr.end(); iter++)
                    table[j++].push_back(iter->first);
            }    
            else
            {
                arr.push_back(std::make_pair(page, ctime));
                totalFilled++;
                
                auto iter = arr.end();
                iter--;
        
                int j = totalFilled - 1;
                int k = 0;
                    
                for (; k < totalFilled; iter--)
                {
                    table[j--].push_back(iter->first);
                    k++;
                }

                for (; k < frame_size; k++)
                    table[k].push_back(-1);
                
            }

            table[frame_size].push_back(1);
            cnt++;
        }
        ctime++;

        /*int j = 0;
        for (auto iter = arr.begin(); iter != arr.end(); iter++)
            table[j++].push_back(iter->first);*/
        
        //std::cout << std::endl;
    }

    std::cout << "No. of PF = " << cnt << "\n\t";

    for (int i = 0; i < frame_size; ++i)
    {
        for (int j = 0; j < len; ++j)
            std::cout << std::setw(2) << table[i][j] << " ";
        std::cout << "\n\t";
    }

    for (int j = 0; j < len; ++j)
        if (table[frame_size][j] == 0) 
            std::cout << std::setw(3) << "- ";
        else
            std::cout << std::setw(3) << "+ ";

    std::cout << std::endl;
}
  

/////////////////////////
