kontakty=read.delim("D:/mrsa/kontakty_chorzy.txt", header = TRUE)
unikalne=unique(kontakty[,1])
czasy<-matrix(0,nrow=length(unikalne),ncol=100)
for (i in 1:length(kontakty[,1])){
  pocz=kontakty[i,12]-kontakty[i,5]
  if (pocz<0 && pocz>-100) {
    pocz=-pocz
    maxi=max(min(max(-kontakty[i,8]+kontakty[i,5],0),max(-kontakty[i,7]+kontakty[i,5],0), 100), pocz-kontakty[i,3])
     for (j in maxi:pocz) {
      czasy[which(unikalne==kontakty[i,1]),j]= czasy[which(unikalne==kontakty[i,1]),j]+1
      }
    }
}