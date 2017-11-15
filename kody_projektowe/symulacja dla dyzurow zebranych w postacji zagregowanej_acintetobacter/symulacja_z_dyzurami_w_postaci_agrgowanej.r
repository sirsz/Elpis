library(igraph)
przesuniecie= 17360 #dyzur liczony jest od 1. Tu wstawiomy wartosc poczatkowa pierwszej wizyty
#symulacja wykorzystujaca dyzury uruchamiana po przekodowaniu (przyklad przekodowania w pliku dyzyry)
wsie_kontakty=read.csv("/dane.csv", sep=";")
wsie_kontakty$data_przyjecia=as.numeric(as.Date(wsie_kontakty$data_przyjecia, format='%d.%m.%Y'))
wsie_kontakty$data_wypisu=as.numeric(as.Date(wsie_kontakty$data_wypisu, format='%d.%m.%Y'))
dyzur=read.csv("/dyzury3.csv", sep=",") 
dyzyur$data_przyjecia=as.numeric(dyzur$$data_przyjecia)+przesuniecie #tu wstawiamy roznice miedzy licznikiem daty z tabali wizyt a tabel¹ rozpoczynajaca sie od 1 z dyzurow
dyzyur$data_wypisu=as.numeric(dyzur$data_wypisu)+przesuniecie
licz=0
place=wsie_kontakty$id_oddzialu
m=unique(place)
rezultat=c(0,0,0,0,0,0,0,0,0,0,0,0)
for (i in 1:length(m)){
  obszar<-which(place==m[i])
  if (length(obszar)>1) {
    kombi=combn(obszar,2)
    for (j in 1:length(kombi[1,])){
      if (wsie_kontakty$data_wypisu[kombi[1,j]]+1>=wsie_kontakty$data_przyjecia[kombi[2,j]] && wsie_kontakty$data_wypisu[kombi[2,j]]+1>=wsie_kontakty$data_przyjecia[kombi[1,j]]) {
        licz=licz+1
        rezultat <- rbind(rezultat, licz+1)
        rezultat[licz+1,1]= wsie_kontakty[kombi[1,j],1]
        rezultat[licz+1,2]= wsie_kontakty[kombi[2,j],1]
        rezultat[licz+1,4]= wsie_kontakty[kombi[1,j],2]
        
        rezultat[licz+1,7]= wsie_kontakty[kombi[1,j],3]
        rezultat[licz+1,8]= wsie_kontakty[kombi[2,j],3]
        
        rezultat[licz+1,11]= wsie_kontakty[kombi[1,j],4]
        rezultat[licz+1,12]= wsie_kontakty[kombi[2,j],4]
        licz=licz+1
        rezultat <- rbind(rezultat, licz+1)
        rezultat[licz+1,1]= wsie_kontakty[kombi[2,j],1]
        rezultat[licz+1,2]= wsie_kontakty[kombi[1,j],1]
        rezultat[licz+1,4]= wsie_kontakty[kombi[1,j],2]
        
        rezultat[licz+1,7]= wsie_kontakty[kombi[2,j],3]
        rezultat[licz+1,8]= wsie_kontakty[kombi[1,j],3]
        
        rezultat[licz+1,11]= wsie_kontakty[kombi[2,j],4]
        rezultat[licz+1,12]= wsie_kontakty[kombi[1,j],4]
        
        if (wsie_kontakty$data_wypisu[kombi[2,j]]>wsie_kontakty$data_wypisu[kombi[1,j]]) {
          durat=min(abs(wsie_kontakty$data_wypisu[kombi[1,j]]-wsie_kontakty$data_przyjecia[kombi[2,j]]), abs(wsie_kontakty$data_wypisu[kombi[2,j]]-wsie_kontakty$data_przyjecia[kombi[1,j]]))+1
        }
        else {
          durat=min(abs(wsie_kontakty$data_wypisu[kombi[2,j]]-wsie_kontakty$data_przyjecia[kombi[1,j]]), abs(wsie_kontakty$data_wypisu[kombi[1,j]]-wsie_kontakty$data_przyjecia[kombi[2,j]]))+1
          
        }
        rezultat[licz,3]= durat
        rezultat[licz+1,3]= durat
      }
      
    }
  }
}
rez2=rezultat[-1,]

place=as.numeric(wsie_kontakty$id_oddzialu)
for i in 1:length(place) {
    for (j in 1:1600){
      if (wsie_kontakty$data_wypisu[i]+1>=dyzur$data_przyjecia[j] && wsie_kontakty$data_wypisu[i]+1>=duzur$data_przyjecia[j]) {
        licz=licz+1
        rezultat <- rbind(rezultat, licz+1)
        rezultat[licz+1,1]= wsie_kontakty[i,1]
        rezultat[licz+1,2]= wsie_kontakty[j,1]
        rezultat[licz+1,4]= wsie_kontakty[kombi[1,j],2]
        
        rezultat[licz+1,7]= wsie_kontakty[kombi[1,j],3]
        rezultat[licz+1,8]= wsie_kontakty[kombi[2,j],3]
        
        rezultat[licz+1,11]= wsie_kontakty[kombi[1,j],4]
        rezultat[licz+1,12]= wsie_kontakty[kombi[2,j],4]
        licz=licz+1
        
        
        if (wsie_kontakty$data_wypisu[kombi[2,j]]>wsie_kontakty$data_wypisu[kombi[1,j]]) {
          durat=min(abs(wsie_kontakty$data_wypisu[kombi[1,j]]-wsie_kontakty$data_przyjecia[kombi[2,j]]), abs(wsie_kontakty$data_wypisu[kombi[2,j]]-wsie_kontakty$data_przyjecia[kombi[1,j]]))+1
        }
        else {
          durat=min(abs(wsie_kontakty$data_wypisu[kombi[2,j]]-wsie_kontakty$data_przyjecia[kombi[1,j]]), abs(wsie_kontakty$data_wypisu[kombi[1,j]]-wsie_kontakty$data_przyjecia[kombi[2,j]]))+1
          
        }
        rezultat[licz,3]= durat
        rezultat[licz+1,3]= durat
      }
      
    }
  }
}
dlugosc=length(rez2[,1])
kontakty <- data.frame(pac1= integer(dlugosc), pac2= integer(dlugosc), trwanie = integer(dlugosc), gdzie = integer(dlugosc), przyjecie1  = integer(dlugosc), przyjecie2= integer(dlugosc))

kontakty$gdzie=rez2[,11]
kontakty$przyjecie2=rez2[,2]
kontakty$przyjecie1=rez2[,1]
kontakty$trwanie=rez2[,3]
kontakty$pac2=rez2[,8]
kontakty$pac1=rez2[,7]


igraphowy2<-data.frame(id1=integer(),id2=integer(),time_s=integer())
m=1

for (i in 1:length(kontakty$pac1)) {
  if (i %% 2 == 0){
    if (kontakty$przyjecie1[i] >= kontakty$przyjecie2[i])
    {
      for(j in 1:kontakty$trwanie[i])
      {
        igraphowy2<-rbind.data.frame(igraphowy2,m)
        igraphowy2$id1[m]=kontakty$pac1[i]
        igraphowy2$id2[m]=kontakty$pac2[i]
        igraphowy2$time_x[m]=kontakty$przyjecie1[i]+j-1
        m=m+1
      }
    }
    else
    {
      for(j in 1:kontakty$trwanie[i])
      {
        igraphowy2<-rbind.data.frame(igraphowy2,m)
        igraphowy2$id1[m]=kontakty$pac1[i]
        igraphowy2$id2[m]=kontakty$pac2[i]
        igraphowy2$time_x[m]=kontakty$przyjecie2[i]+j-1
        m=m+1
      }
    }
  }
}
igraphowy2$X1 <- NULL



# zakazalnosc
betas = c(0.25,0.15,0.20,0.50,0.1) #zakazalnosc dla acinetobactera
# immunologia
gamma = 0.1 # dekolonizacja dla acinetobactera
# dlugosc trwania epidemii
n_timesteps = 32
# z mikrobiologii
kk=as.numeric(wsie_kontakty$Data.rejestracji)
zbior2=which(kk>1)
seeds=wsie_kontakty$id_pacjenta[zbior2]
macierz=matrix(0,nrow=length(unique(wsie_kontakty$id_pacjenta)),ncol=length(unique(wsie_kontakty$id_pacjenta)))
kto=c(0,seed,0)
  licz=0
#ilosc symulacji
for (k in 1:100000){
  seed=sample(seeds, 1)
  beta=sample(betas, 1)
  
  infected_nodes<- vector()
  infected_nodes=append(infected_nodes, seed)
  

  for (i in 17361:17371){
    # Infection stage
    for (node in infected_nodes){
      gt <- delete_edges(g,which(E(g)$time_x != i))
      if (length(E(gt))>0) {
      for (neighbor in neighbors(gt,as.character(node))){
        # random.random simply returns a number between [0,1)
        if (runif(1) < beta && (!(V(g)[neighbor]$name %in% infected_nodes))) {
          infected_nodes=append(infected_nodes, V(g)[neighbor]$name)
          licz=licz+1
          kto<-rbind(kto,licz+1)
          kto[licz+1,1]=node
          kto[licz+1,2]=V(g)[neighbor]$name
          kto[licz+1,3]=i
          macierz[match(node, V(g)$name), neighbor]=macierz[match(node, V(g)$name), neighbor]+1
                  }
      }
      }
    }
  }
}
for (k in 1:100000){
  seed=sample(seeds, 1)
  beta=sample(betas, 1)/5
  
  infected_nodes<- vector()
  infected_nodes=append(infected_nodes, seed)
  
  for (i in 17361:17371){
    # Infection stage
    for (node in infected_nodes){
      gt <- g
      if (length(E(gt))>0) {
      for (neighbor in neighbors(gt,as.character(node))){
        # random.random simply returns a number between [0,1)
        if (runif(1) < beta && (!(V(g)[neighbor]$name %in% infected_nodes))) {
          infected_nodes=append(infected_nodes, V(g)[neighbor]$name)
          licz=licz+1
          kto<-rbind(kto,licz+1)
          kto[licz+1,1]=node
          kto[licz+1,2]=V(g)[neighbor]$name
          kto[licz+1,3]=i
          macierz[match(node, V(g)$name), neighbor]=macierz[match(node, V(g)$name), neighbor]+1
                  }
      }
      }
    }
  }
}
net=graph_from_adjacency_matrix(macierz,  weighted=TRUE, mode="directed")
write.csv(macierz, "macierz_.csv")
ostateczny=betweenness(net, directed=T)
write.csv(ostateczny, "ostateczny_.csv")
kody=V(g)$name
write.csv(kody, "kody_.csv")
write.csv(kto, "kto_.csv")
for (node1 in seeds){
V(net)[which(V(net)$name==node1)]$chorzy=1
}
pdf("sieci_nie3.pdf")
plot(net, edge.arrow.size=.25, vertex.size=7, vertex.label.cex=0.7, vertex.color=c( "pink", "skyblue")[1+(V(net)$chorzy==1)])
dev.off()
