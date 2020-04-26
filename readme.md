## Header-only Entity-Component-System.

For POD components: 
If a component owns resources then it must have a 'free()' member function to make the storage handles the freeing.
