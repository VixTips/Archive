//author vic
public class JavaApplication1 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
          int[] i = {40,60,10,20,50,30};
        int[] i_2 = {1,2,4,6,2,8,6,5};
          MaxHeap myHeap = new MaxHeap(i);
          
          myHeap.heapToString();
          
          myHeap.makeMaxHeap();
          myHeap.heapToString();
          
         
	  System.out.println("\nAdding integer 24:");
          myHeap.insert(34);
          myHeap.heapToString();
          
          System.out.println("\ndeleting: " + myHeap.deleteMax());
         myHeap.heapToString();
          
          myHeap.heapSort(i);
         System.out.println("\nSorting:");
         myHeap.heapToString();
         
         myHeap.heapSort(i_2);
          System.out.println("\nSorting array 2:");
         myHeap.heapToString();
 
         myHeap.getArraySize();
         myHeap.getHeapSize();
         
         myHeap.insert(34);
          myHeap.getArraySize();
         myHeap.getHeapSize();
         
                   myHeap.insert(45);
          myHeap.insert(60);
          
                    myHeap.getArraySize();
         myHeap.getHeapSize();
    }
    
}
