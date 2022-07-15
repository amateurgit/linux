/* json 对象被多个对象引用时，控制引用计数来释放内存 */

/*
Step 1:
   json_object 对象 A, B, C

Step 2:
   json_object_object_add(B, "obj1", A); 
   json_object_object_add(C, "obj2", A);
   json_object_get(A);	// A 引用计数加 1

Step 3:
   json_object_put(B);
   json_object_put(C);
 */
