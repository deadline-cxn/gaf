# GAF (GAME ARCHIVE FILE)

GAF Game Archive File by @sparson aka Deadline sethcoder.com

This application is based on the NukeDX SDK NUK file format by Odin Jensen from nukesoftware.dk (Which is no longer available) 

###### NOTE: This archive format only nests one level of folder


Current version: 5.0.0


### USAGE:

```
gaf <gaf file> [-a:<filename>] [-z:<dir>] [-d:<file>] [-c:<dir>] [-x]

-a add a file to the GAF (This will create a new GAF if it does not exist)<br>
-z add directory to the GAF (This will create a new GAF if it does not exist)<br>
-d remove a file from the GAF<br>
-c remove a directory from the GAF<br>
-x extract GAF into the working directory<br>
-l list contents<br>
```

##### The DLSTORM library is required to compile
