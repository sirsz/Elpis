library(ggplot2)
library(igraph)
library(sp)
load('podstawowe.Rdata')
#wczystuje mape Polski oraz informacje o ludnosci
seedsy=c(203) # wskazujemy zakazony powiat
kto=c(0,0,0,-1,0)
beta=.2
inf=10
p1=matrix(0, nrow=380, ncol = 380)
#mamy w Polsce 389 powiatow
for (j in 1:379) {
  for (i in (j+1):380){
    p1[i,j]=inf*(beta*(V(net2)$lud[i]*V(net2)$lud[j])/(1+macierz[i,j]))
  }
}

for (k in 1:1){
#krok symulacji tydzien
#model rorzprzestrzeniania ~inf*beta(H(1)*H(2))/(1+d(1,2))

infected_nodes<- vector()
infected_nodes=append(infected_nodes, seedsy)
kto<-rbind(kto,c(0,seedsy[1],0,seedsy[1]-1,k))

licz=1
for (i in 1:n_timesteps){
  # Infection stage
  for (node in infected_nodes){
    r=ceiling(380*runif(1))
    if (runif(1) < (p1[node,r]+p1[r,node])/(100) && (!(r %in% infected_nodes))) {
      infected_nodes=append(infected_nodes, r)
      licz=licz+1
      kto<-rbind(kto,licz+1)
      kto[licz+1,1]=node
      kto[licz+1,2]=r
      kto[licz+1,3]=i
      kto[licz+1,4]=r-1
      kto[licz+1,5]=k
    }
    for (neighbor in neighbors(net2,node)){
  # random.random simply returns a number between [0,1)
    if (runif(1) < (p1[neighbor,node]+p1[node, neighbor])/50 && (!(neighbor %in% infected_nodes))) {
      infected_nodes=append(infected_nodes, neighbor)
      licz=licz+1
      kto<-rbind(kto,licz+1)
      kto[licz+1,1]=node
      kto[licz+1,2]=neighbor
      kto[licz+1,3]=i
      kto[licz+1,4]=neighbor-1
      kto[licz+1,5]=k

    }
    }
  }
}
}
# Removal stage

zakazenia2=data.frame(idki=c(0:379))
zakazenia2$czas=NA
for (licznik in 1:length(kto[,1]))
{zakazenia2$czas[kto[licznik,2]]=kto[licznik,3]
}
nasz <- inner_join(ploty,zakazenia2, by = c("id" = "idki"))
 p<-ggplot(data = nasz, mapping = aes(x = long, y = lat)) +
   geom_polygon(mapping = aes(group = group, fill = czas.y))+
   scale_fill_distiller("risk", palette = "YlGn", breaks = pretty_breaks(n = 7), trans = "reverse")
 
 pdf("ndm_z_warszawy.pdf")
  p
  dev.off()