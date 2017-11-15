for (i in 1:9) {
  kto=nowe_kontakty$ID.2[i]
  gdzie=nowe_kontakty$ward[i]
  tek<-subset(wsie_kontakty,wsie_kontakty$V2==kto)
  tek<-subset(tek, tek$V3==gdzie)
  nowe_kontakty[i,7]=tek[1,6]
  nowe_kontakty[i,8]=tek[1,4]
  nowe_kontakty[i,9]=tek[1,5]
}