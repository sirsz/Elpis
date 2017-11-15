people=kontakty[,2]
unikalne=unique(people)
macierz=matrix(0,nrow=1000,ncol=55)
for (i in 1:length(unikalne)){
  obszar<-which(people==unikalne[i])
  wektor=NA
  for (j in obszar){
    
    wektor[j]=kontakty[j,4]-kontakty[j,6]
  }
    wektor2=wektor[!is.na(wektor)]
   wektor3=wektor2[wektor2<0]
   wektor3=wektor3[wektor3>-1000]
   wektor4=abs(wektor3)
   wektor_l=sort(wektor4)
   licz=1
   for (k in wektor_l) {
     macierz[k,licz]=macierz[k,licz]+1
     licz=licz+1
   }
  
}
