people=kontakty[,2]
unikalne=unique(people)
rozklad=NA
for (i in 1:length(unikalne)){
  obszar<-which(people==unikalne[i])
  wektor=NA
  for (j in obszar){
    wektor[j]=kontakty[j,5]-kontakty[j,6]
    wektor2=wektor[!is.na(wektor)]
   wektor3=wektor2[wektor2>=0]
  }
  rozklad[i]=min(wektor3)
}
rozklady=ecdf(rozklad)