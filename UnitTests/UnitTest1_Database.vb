Imports System.Text
Imports Microsoft.VisualStudio.TestTools.UnitTesting



Imports System.Data.SQLite

<TestClass()> Public Class UnitTest1_Database

    <TestMethod()> Public Sub Database_Test1_GetLogs()
        Console.WriteLine("Testmethod1 was launched")
        Console.WriteLine("DbSqliteInterface")
        'Dim commMakeDb As SQLiteConnection = New SQLiteConnection("C:\Users\Sebastian\Documents\Projekty\Andrzeja-Elpis\Tests\SQLiteDbTests\ErisHistorian.sqlite")
        'Dim commMakeDb As SQLiteConnection = New SQLiteConnection("VbErisHistorian.sqlite")
        Dim conn = New SQLiteConnection("Data Source=C:\Users\Sebastian\Documents\Projekty\Andrzeja-Elpis\Tests\SQLiteDbTests\ErisHistorian.sqlite;Version=3")
        conn.Open()

        Dim sqlQuerry As String = "SELECT *FROM LOGS;"
        Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, conn)
        Dim sqlReader As SQLiteDataReader
        Try
            'cmdSqlite.ExecuteNonQuery()
            sqlReader = cmdSqlite.ExecuteReader()
        Catch ex As Exception
            Console.WriteLine("[DbSqliteInterface] Error occured" + ex.ToString())
        End Try
        While (sqlReader.Read())
            Console.WriteLine("Row: " & sqlReader("id") & " " & sqlReader("timestamp") & " " & sqlReader("type") & " " & sqlReader("descr"))
        End While
        conn.Close()

    End Sub

End Class