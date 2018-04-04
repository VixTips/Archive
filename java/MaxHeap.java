//author vic

public class MaxHeap {
        int[] myHeap;
        //number of elements in the array
        int sizeHeap;
    
        //the size of the array
        int arraySize;
        
        //done
        public  MaxHeap(int[] input)
        {
                arraySize = input.length;
                sizeHeap = input.length;
                myHeap = input;            
        
        }
        
        public int[] MaxHeap(int[] input)
        {
                 arraySize = input.length;
                sizeHeap = input.length;
                myHeap = input;   
                return myHeap;
        }
        
        //done
        public void Heapify(int index)
        {
                    int parent = index;
                    int leftChild = (index*2)+1;
                    int rightChild = (index*2)+2;

                    if (leftChild<sizeHeap && myHeap[index] < myHeap[leftChild])
                    {
                            //new target index
                            parent = leftChild;              
                    }
                    if (rightChild<sizeHeap && myHeap[parent] < myHeap[rightChild])
                    {
                            //new target index
                            parent = rightChild;
                    }
                    if (parent != index)
                    {
                        //if the target node changed, swap the elements
                        swap(parent, index);
                        Heapify(parent);
                    }
        
          }
        
        //done
        public void makeMaxHeap()
        {
                    for(int i = (sizeHeap/2)-1; i>=0;i--)
                    {
                              Heapify(i);
                    }
        
        
        }
        //done
        public void swap(int index1, int index2)
        {
                int temp = myHeap[index1];
                myHeap[index1] = myHeap[index2];
                myHeap[index2] = temp;        
        }
        
        //done
        public void heapToString()
        {
                for(int i=0;i<sizeHeap;i++)
                {
                              System.out.print(myHeap[i] + " , ");                
                }
                System.out.println();
                
        }
              
        
        public int deleteMax()
        {
            //get the value
            int temp = myHeap[0];
            myHeap[0] = myHeap[sizeHeap-1];
            sizeHeap--;
            Heapify(0);
            return temp;
        }
        //must be static???
        public void heapSort(int []input)
        {
            // "call" the constructor...
                arraySize = input.length;
                sizeHeap = input.length;
                myHeap = input;  
            
                int[] tempArray = new int[sizeHeap];
                int j = 0;   
                 int temp = sizeHeap;
                 
                 makeMaxHeap();
                 
                for(int i = 0;i<temp;i++)
                {
                    tempArray[i] = this.deleteMax();
                    makeMaxHeap();               
                    
                   // swap(0,i);
                    //sizeHeap--;
                    //Heapify(0);
                }
                sizeHeap = temp;
                myHeap = tempArray;
        }
        
        public void insert(int n)
        {
            
            
            //if the heap size larger than space in array, make new array double the size
            if((sizeHeap +1)>= arraySize)
            {
                int[] temp = myHeap;
                myHeap = new int[arraySize*2];
                for(int i=0; i<sizeHeap; i++) 
                { 
                    myHeap[i] = temp[i];
                }
                arraySize *=2;
            }
           
            int temp = sizeHeap;
            myHeap[temp] = n;
            //mistake here
            while(temp >1 && myHeap[(temp-1)/2] < myHeap[temp])
            {
                    swap((temp-1)/2, temp);
                    temp = (temp-1)/2;
            }     
            
            sizeHeap++;
        }
        
        public void getHeapSize()
        {
            System.out.println("Heap size = " + sizeHeap);
        }
        
        public void getArraySize()
        {
            System.out.println("Array size = " + arraySize);
        }
        
}
