typy=lista_chorych[,4]
por=kontakty[,5]
stala=kontakty[,4]
people=lista_chorych[,2]
for (j in 1:50) {
  typyw=sample(typy, length(typy), replace = FALSE, prob = NULL)
  lista_chorych[,4]=typyw

  for (i in 1:length(kontakty[,1])){
    ktory=which(people==kontakty[i,2])
    por[i]=lista_chorych[ktory,4]
  }
  licz=0
  graf=c(0,0)
  lista=which(por==stala)
  for (licznik in lista) {
    licz=licz+1
    graf <- rbind(graf, licz)
    graf[licz,1]=kontakty[licznik,1]
    graf[licz,2]=kontakty[licznik,2]
  
  }
  g=graph.data.frame(graf,directed=FALSE)
  ll[j]=length(V(g))

  odpowiedz[j]=length(which(por==stala))
}
  