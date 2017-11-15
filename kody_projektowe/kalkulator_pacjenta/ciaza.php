<html>
<head>
    <title>Badanie ryzyka zakazenia szpitalnego</title>


</head>
<body>
<img src="logo2.png" alt="SIRSZ logo" align="left"><font color=#18453B size=1><br> System Informatyczny Redukcji Szpitalnych Zakazen <br>
Wyznaczanie ryzyka zakazenia szpitalnego<br>
Poznaj prawdopodobienstwo zakazenia </font>
 
 
 
<h3> Macierzynstwo 2.0 </h3>



<?


$Q1=	0;
$Q2=	0;
$Q3=    0;
$Q4=	0;
$Q5=	0;
$Q6=	0;
$Q7=	0;
$Q8=	0;
$Q9=	0;
$Q10=	0;
$Q11=	0;
$Q12=	0;
$Q13=	0;
$Q14=	0;
$Q15=	0;
$Q16=	0;
$Q17=	0;
$Q18=	0;
$Q19=	0;
$Q20=	0;
//sprawdzanie galezi porod naturalny/ciecie
$Q21=0;

$Q22=	1;
$Q23=	1;
//pytanie liczbowe
$Q24=	12;

//pytanie nominalne
$Q25_c=	0;
$Q25_cs=1;
$Q25_s=	1;
$SUM=$Q1+$Q2+$Q3+$Q4+$Q5+$Q6+$Q7+$Q8+$Q9+$Q10+$Q11+$Q12+$Q13+$Q14+$Q15+$Q16+$Q17+$Q18+$Q19+$Q20;

if ($Q21==0)
{
$Z=-$SUM*0.03-0.02;
$prob=100*(1/(1+EXP($Z))-0.5)*2;
}
else
{
$Z=-0.035*$SUM-0.04-0.15*$Q25_cs-0.15*$Q25_s-0.03*$Q22+0.05*$Q23-0.08*$Q23*$Q22-$Q24*0.00013-$Q13*0.34;
$prob=100*(1/(1+EXP($Z))-0.5)*2;
}

echo "<h1> prawdopodobienstwo zakazenia szpitalnego ".$prob."%</h1> </p>";



?>
<p>


</body>
</html>
		