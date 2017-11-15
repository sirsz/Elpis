kontakty=read.delim("D:/mrsa/wizyty.txt", heade=TRUE)
czasy<-array(0,dim=c(1042,2600,2))
for (i in 1:length(kontakty[,1])){
  for (j in kontakty[i,3]:kontakty[i,4]) {
      czasy[which(unikalne2==kontakty[i,2]),j-8000,1] = czasy[which(unikalne2==kontakty[i,2]),j-8000,1]+1
      czasy[which(unikalne2==kontakty[i,2]),j-8000,2] = kontakty[i,6]
      
  }
}