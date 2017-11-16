require(igraph)
#tabela wizyty
message("Start")

## locale-specific version of date()
format(Sys.time(), "%a %b %d %X %Y")

message("Read file")
# exported/visit_oryg.csv is replacing exported/visit.csv
wsie_kontakty=read.csv("exported/visit.csv", sep = ";")
message("Visits Loaded")
wsie_kontakty$data_przyjecia=as.numeric(as.Date(wsie_kontakty$data_przyjecia, format='%Y-%m-%d'))
wsie_kontakty$data_wypisu=as.numeric(as.Date(wsie_kontakty$data_wypisu, format='%Y-%m-%d'))
#pom=as.numeric(wsie_kontakty$data_wypisu)
#wsie_kontakty$data_przyjecia=pom
#pom=as.numeric(wsie_kontakty$data_przyjecia)
#wsie_kontakty$data_wypisu=pom



licz=0
place=wsie_kontakty$id_oddzialu
m=unique(place)
pacjenci=unique(wsie_kontakty$id_pacjenta)
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
dlugosc=length(rez2[,1])
kontakty <- data.frame(pac1= integer(dlugosc), pac2= integer(dlugosc), trwanie = integer(dlugosc), gdzie = integer(dlugosc), przyjecie1  = integer(dlugosc), przyjecie2= integer(dlugosc))

kontakty$gdzie=rez2[,11]
kontakty$przyjecie2=rez2[,2]
kontakty$przyjecie1=rez2[,1]
kontakty$trwanie=rez2[,3]
kontakty$pac2=rez2[,8]
kontakty$pac1=rez2[,7]
m=1
igraphowy2<-data.frame(id1=integer(),id2=integer(),time_s=integer())

dyzury=read.csv("exported/duty.csv", sep = ";")
personel=dyzury$id_personelu
daty_dyz=as.numeric(as.Date(dyzury$data_dyżuru, format='%Y-%m-%d'))

for (ind in 1:length(personel)){
 pozniej<-which(wsie_kontakty$data_wypisu>=daty_dyz[ind])
 wczesniej<-wsie_kontakty$data_przyjecia[pozniej]
 porownanie<-which(wczesniej<=daty_dyz[ind])
 if (length(porownanie)>0) {
  for (kol in porownanie){
    igraphowy2<-rbind.data.frame(igraphowy2,m)
    igraphowy2$id1[m]=personel[ind]
    igraphowy2$id2[m]=wsie_kontakty$id_pacjenta[kol]
    igraphowy2$time_x[m]=daty_dyz[ind]
    m=m+1
  }
}
}


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

message("Start algorytm")
igraphowy2$X1 <- NULL

g <- graph.data.frame(igraphowy2,directed=F)
infected_nodes<- vector()
# zakazalnosc
betas = c(0.25,0.15,0.20,0.50,0.1)
beta = 0.3
# immunologia
gamma = 0.1
# dlugosc trwania epidemii
n_timesteps = 32
# z mikrobiologii
mikrobiologia=read.csv("exported/test.csv", sep =";", header=FALSE)
seeds=mikrobiologia[,3]
seed=seeds[1]

#tryb losowy
pacjenci2=length(pacjenci)+length(unique(personel))
#seed=sample(pacjenci2, 1)

#ilosc symulacji
sym=5
kto=c(0,which(V(g)$name==seed),0,symulacje, beta*100)
licz=0
macierz=matrix(0,nrow=pacjenci2,ncol=pacjenci2)

for (symulacje in 1:sym){
  message("sym")
  beta=sample(betas, 1)
  for (k in 1:(length(seeds))){
    if (k>1) 
      {seed=sample(seeds, 1)}
    infected_nodes<- vector()
    infected_nodes=append(infected_nodes, which(V(g)$name==seed))
    for (i in min(wsie_kontakty$data_przyjecia):max(wsie_kontakty$data_wypisu)){
    # Infection stage
      for (node in infected_nodes){
        gt <- delete_edges(g,which(E(g)$time_x != i))
        if (length(E(gt))>0) {
          for (neighbor in neighbors(gt,node)){
          # random.random simply returns a number between [0,1)
            if (runif(1) < beta && (!(neighbor) %in% infected_nodes)) {
              infected_nodes=append(infected_nodes, neighbor)
              licz=licz+1
              kto<-rbind(kto,licz+1)
              kto[licz+1,1]=node
              kto[licz+1,2]=neighbor
              kto[licz+1,3]=i
              kto[licz+1,4]=symulacje
              kto[licz+1,5]=beta*100
              macierz[node, neighbor]=macierz[node, neighbor]+1
            }
          }
        }
      }
    }
  }
}
net=graph_from_adjacency_matrix(macierz,  weighted=TRUE,mode="directed")
ostateczny=betweenness(net, directed=T)
message("Write to file")
# Relative Working Sirectory path is used - instead of the global one:
#setwd("C://Users/Damian/Desktop/siralgorytm/")
setwd(getwd())
getwd()
write.csv(ostateczny, "result/report-visits.csv")

for (node1 in seeds){
  V(net)[which(V(g)$name==node1)]$chorzy=1
}
pdf("result/sieci_nie2.pdf")
plot(net, edge.arrow.size=.25, vertex.size=7, vertex.label.cex=0.7, vertex.color=c( "pink", "skyblue")[1+(V(net)$chorzy==1)], edge.width=E(net)$weight)
dev.off()

message("END")
