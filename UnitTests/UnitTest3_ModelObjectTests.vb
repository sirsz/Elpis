Imports System.Text
Imports Microsoft.VisualStudio.TestTools.UnitTesting

<TestClass()> Public Class UnitTest3_ModelObjectTests


    <TestMethod()> Public Sub WardClear_Test()
        Dim ward As New Elpis.Ward()
        ward.Clear()
        Dim expected_value As UInteger = 0
        Assert.AreEqual(ward.f_id_, expected_value)

    End Sub

    <TestMethod()> Public Sub WardFormCreateTableQuerry_Test()

        Dim querry As String = Elpis.Ward.FormQuerry_DropAndCreateTable()
        Console.WriteLine(querry)
        Assert.IsTrue(querry.Length > 0)
    End Sub

    <TestMethod()> Public Sub WardFormSelectQuerry_Test()
        Dim ward As New Elpis.Ward
        Dim querry As String = ward.FormQuerry_Select
        Console.WriteLine(querry)
        Assert.IsTrue(querry.Length > 0)
    End Sub

    <TestMethod()> Public Sub WardFormInsertQuerry_Test()
        Dim ward As New Elpis.Ward
        Dim querry As String = ward.FormQuerry_Insert()
        Console.WriteLine(querry)
        Assert.IsTrue(querry.Length > 0)
    End Sub

    <TestMethod()> Public Sub WardFormUpdateQuerry_Test()
        Dim ward As New Elpis.Ward
        Dim querry As String = ward.FormQuerry_Update()
        Console.WriteLine(querry)
        Assert.IsTrue(querry.Length > 0)
    End Sub

    <TestMethod()> Public Sub WardFormDeleteQuerry_Test()
        Dim ward As New Elpis.Ward
        Dim querry As String = ward.FormQuerry_Delete()
        Console.WriteLine(querry)
        Assert.IsTrue(querry.Length > 0)
    End Sub

End Class