Imports System.Text
Imports Microsoft.VisualStudio.TestTools.UnitTesting
Imports Microsoft.VisualBasic.DateAndTime

<TestClass()> Public Class UnitTest2_Context

    <TestMethod()> Public Sub Context_TestMethod0()
    End Sub

    <TestMethod()> Public Sub Context_NotInitialized()
        Dim c As New Elpis.Context
        Assert.IsTrue(c.currHospital_ Is Nothing)
    End Sub

    <TestMethod()> Public Sub Context_Initialized()
        Dim c As New Elpis.Context
        Dim hosp As New Elpis.Hospital(1, "miejski")

        c.UpdateHospital(hosp)
        Assert.IsFalse(c.currHospital_ Is Nothing)
    End Sub

    <TestMethod()> Public Sub DateFormatTest1()

        Dim data1 As Date

        Dim data1_str As String = "2015-01-02"
        data1 = data1_str
        Dim date1_conv_to_string As String = data1.ToString
        Console.WriteLine(date1_conv_to_string)
        Assert.IsTrue(data1.Year.Equals(2015))
    End Sub

End Class