public class testClass
{
	testClass(){System.out.println("Begin Testing");}	

	public void test()
	{
		int[] i = {40,60,10,20,50,30};
    int[] i_2 = {1,2,4,6,2,8,6,5};
      	

    System.out.println("\nBuild empty heap");
    MaxHeap myHeap = new MaxHeap(10);
    myHeap.heapToString();
    myHeap.getArraySize();
    myHeap.getHeapSize();


    System.out.println("\nBuild heap from array i");
    myHeap = new MaxHeap(i);    


    System.out.println("\nPrint array i"); 
    myHeap.heapToString();        
    myHeap.getArraySize();
    myHeap.getHeapSize();


    System.out.println("\nBuild the max heap");  
    myHeap.makeMaxHeap();
    myHeap.heapToString();


	  System.out.println("\nAdd integer 34:");
    myHeap.insert(34);
    myHeap.heapToString();


    System.out.println("\nAdd integer 1:");
    myHeap.insert(1);
    myHeap.heapToString();


    System.out.println("\nDelete: " + myHeap.deleteMax());
    myHeap.heapToString();


    System.out.println("\nSorting ipnut array i:");
    myHeap.heapSort(i);
    myHeap.heapToString();
            	

    System.out.println("\nSorting input array i_2:");
    myHeap.heapSort(i_2);
    myHeap.heapToString();
    myHeap.getArraySize();
    myHeap.getHeapSize();
  

    System.out.println("\nInsert 34 into array i_2");
    myHeap.insert(34);
    myHeap.getArraySize();
    myHeap.getHeapSize();

         
    System.out.println("\nInsert 45 and 60 into array i_2:"); 
    myHeap.insert(45);
    myHeap.insert(60);         
    myHeap.getArraySize();
    myHeap.getHeapSize();

    myHeap.heapToString();
	}
}