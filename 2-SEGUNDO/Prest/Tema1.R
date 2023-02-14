################################
# Define the working Directory #
################################
setwd("/home/enrique/Descargas/Prest")
#############################
# Install and Load Packages #
#############################
#install.packages("foreing")
#install.packages("qcc")
library(foreign)
library(qcc)
#####################
# Loading data sets #
#####################
Fibre = as.data.frame(read.spss("fibre.sav", rownames=FALSE,stringsAsFactors=TRUE, tolower=FALSE))
msoftware = read.table("msoftware.txt",header=TRUE, sep=" ",na.strings="NA", dec=",", strip.white=TRUE)
CPU = read.table("cpu.dat",header=TRUE, sep="\t", na.strings="NA",dec=".", strip.white=TRUE)
transistor=read.table("transistor.txt",header=TRUE, sep="\t",
                       na.strings="NA", dec=",", strip.white=TRUE)                        
# Frecuency table
absfreq = table(Fibre$machine)
relfreq = absfreq/ sum(absfreq)
percfreq = relfreq * 100
cbind(absfreq, relfreq, percfreq)

# Pie chart
pie(table(Fibre$machine))

# Bar chat
barplot(table(Fibre$machine),cex.names=0.65)

# Pareto Chart
df = as.data.frame(table(msoftware))
pareto.chart(df$Freq)

######################
# Discrete Variables #
######################
absfreq = c(3,11,9,20,5,2)
relfreq = absfreq/sum(absfreq)
cumabsfreq = cumsum(absfreq)
cumrelfreq = cumsum(relfreq)
t = round(cbind(absfreq,relfreq,cumabsfreq,cumrelfreq),4)
nans = 0:5
tfreq = cbind(nans,t)
barplot(absfreq,xlab="Number correct answers",names.arg=nans,ylab="
Absolute frequency")
# Histogram (used in discrete variables or continuous)
hist(log(CPU$eff),freq=FALSE,xlab="log(CPU efficiency)",main="")
hist(CPU$eff,freq=FALSE,xlab="CPU efficiency",main="")
boxplot(CPU$eff,main="CPU efficiency")
boxplot(log(CPU$eff),main="log(CPU efficiency)")
###############################
# Medidas de posicion central #
################################
#mean by hand:
xi = 1:20
ni = c(5, 4,4,4,3,4,1,4,0,4,2,2,2,1,1,3,1,0,0,1)
N = sum(ni)
fi = ni/N
mean_a = sum(xi*ni)/N
mean_b = sum(fi*xi)
mean_a; mean_b
# median
y = sort(rep(xi,ni))
mean(y); median(y)
# quartiles
quantile(y, probs = seq(0, 1, by=0.25))
# Deciles
quantile(y, probs = seq(0,1, by = 0.1))
# Percentiles
quantile(y, probs = seq(0, 1, by = 0.01))
# Find the 37th, 53rd, and 87th percentiles
quantile(y, probs = c(0.37, 0.53, 0.87))
# Quartiles using data set CPU
quantile(CPU$eff, probs = seq(0, 1, by=0.25))
# 35th percentile
quantile(CPU$eff, probs = 0.35)
# Quantile of order 0.125
quantile(CPU$eff, probs = 0.125)
#boxplot
par(mfrow = c(1, 2))
boxplot(CPU$eff,main="CPU efficiency")
boxplot(log(CPU$eff),main="log(CPU efficiency)")


RegModel.9<-lm(y~x,data=transistor)
summary(RegModel.9)

x9 = transistor$x
y9 = transistor$y
plot(x9,y9,xlab="voltaje de tierra a fuente",ylab="coriente de
drenaje",pch =16)
abline(lm(y9 ~ x9, data = transistor), col = "red")

# Linear regression by hand
x=c(30, 28, 32, 25, 25, 25, 22, 24, 35, 40)
y=c(25, 30, 27, 40, 42, 40, 50, 45, 30, 25)
beta1=cov(x,y)/var(x)
beta0=mean(y)-beta1*mean(x)
# Linear regression using R's function
mod=lm(y~x)
summary(mod)

