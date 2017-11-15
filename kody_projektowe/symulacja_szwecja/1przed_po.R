kontakty=read.delim("D:/mrsa/kontakty_chorzy.txt", header = TRUE)
unikalne=unique(kontakty[,1])
czasy<-matrix(0,nrow=length(unikalne),ncol=100)
for (i in 1:length(kontakty[,1])){
  pocz=kontakty[i,8]-kontakty[i,5]
  if (pocz>0 && pocz<100) {
    maxi=min(max(kontakty[i,9]-kontakty[i,5],0),max(kontakty[i,13]-kontakty[i,5],0),pocz+kontakty[i,3],100)
     for (j in pocz:maxi) {
      czasy[which(unikalne==kontakty[i,1]),j]= czasy[which(unikalne==kontakty[i,1]),j]+1
      }
    }
}