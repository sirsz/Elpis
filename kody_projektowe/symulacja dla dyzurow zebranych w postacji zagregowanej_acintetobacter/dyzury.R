#schemat kodowania przypisow pracownikow do oddzialu z tabeli zebranej podczas inicjacji
piel_alergia=subset(prac, miejsce==1)
piel_alergia=subset(piel_alergia, grupa=="p")
piel_intensywna=subset(prac, miejsce==2)
piel_intensywna=subset(piel_intensywna, grupa=="p")
piel_pulm=subset(prac, miejsce==3)
piel_pulm=subset(piel_pulm, grupa=="p")
dyzury <- data.frame(data_przyjecia= integer(0), data_wypisu= integer(0), id_personel = integer(0), odzial = integer(0), kod  = character(0))
for (i in 1:40)
{
  dyzur=sample(piel_alergia$ewidencja, 14)
     for (j in 1:length(dyzur)) {
       dyzury[nrow(dyzury) + 1,]=c(i,i,dyzur[j],1, 'AIOKL') 
   }
  dyzur=sample(piel_intensywna$ewidencja, 8)
  for (j in 1:length(dyzur)) {
    dyzury[nrow(dyzury) + 1,]=c(i,i,dyzur[j],2, 'Inte') 
  }
  dyzur=sample(piel_pulm$ewidencja, 18)
  for (j in 1:length(dyzur)) {
    dyzury[nrow(dyzury) + 1,]=c(i,i,dyzur[j],3, 'Pulm') 
  }
}
