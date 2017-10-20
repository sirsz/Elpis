Imports System.Data.SQLite
Imports System.Data


Public Class DbSqliteInterface
    'Private Sub binToDb_Click(sender As Object, e As RoutedEventArgs) Handles bntDb.click

    Dim dataBaseName_ As String = "ElpisDb.db3"
    Dim IsDataBaseInitialized_ As Boolean = False

    Dim ConnectionString_ As String
    Dim IsDataBaseOpened_ As Boolean = False
    Dim isSetUp_ As Boolean = False

    Dim Connection_ As SQLiteConnection

    ' Should be callsed when Application is set up.
    Public Function SetUp(ByVal path_to_data_base As String) As Boolean

        Dim result As Boolean = False
        'If Not is_database_initialized Then
        '    IsDataBaseInitialized_ = CreateNewDataBase(path_to_data_base)
        'End If
        dataBaseName_ = path_to_data_base
        ConnectionString_ = "DataSource=" + dataBaseName_ + ";Version=3;New=False;Compress=True;"

        If Connection_ Is Nothing Then
            Connection_ = New SQLiteConnection
            Connection_.ConnectionString = ConnectionString_
            isSetUp_ = True
        End If

        result = ChangeDataBase(path_to_data_base)

        Return isSetUp_

    End Function

    ' Simple database connection test, returns true if using database is possible, false otherwise
    Public Function TestDbConnection() As Boolean

        ' 1/2.: Check if can open connection
        Dim couldOpenDbConnection = False
        Try
            Connection_.Open()
            couldOpenDbConnection = True
        Catch ex As Exception
            ' If file has wron format 
            'Console.WriteLine("[DbSqliteInterface] Error database file has wrong format or was not initialized " + ex.ToString())
            Toolbox.Log("[DbSqliteInterface] Error database file has wrong format or was not initialized " + ex.ToString())
            ' Wyglada na to, ze to nie zadziala.
            couldOpenDbConnection = False
        End Try
        If couldOpenDbConnection = False Then
            Return couldOpenDbConnection
        End If

        ' 2/2.: Check if can querry DB
        Dim couldQueryDbConnection = False
        Dim sqlQuerry As String = "SELECT ID FROM WARD;"
        Dim sqlReader As SQLiteDataReader
        Try
            Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, Connection_)
            sqlReader = cmdSqlite.ExecuteReader()
            couldQueryDbConnection = True
        Catch ex As Exception
            'Console.WriteLine("[DbSqliteInterface] Error can't querry DB " + ex.ToString())
            Toolbox.Log("[DbSqliteInterface] Error can't querry DB " + ex.ToString())
            couldQueryDbConnection = False
        End Try
        Connection_.Close()

        ' Return test result
        Return couldOpenDbConnection And couldQueryDbConnection
    End Function

    ' Function prepared for serving button with DataBase path:
    Public Function ChangeDataBase(ByVal path_to_data_base As String) As Boolean
        Dim result As Boolean = False

        If Not My.Computer.FileSystem.FileExists(path_to_data_base) Then
            ' If file not exists - create new one:
            result = CreateNewDataBase(path_to_data_base)
        Else
            ' If file exists - just check if you can use them:
            Dim ConnectionString_saved = ConnectionString_
            ConnectionString_ = "DataSource=" + path_to_data_base + ";Version=3;New=False;Compress=True;"
            Connection_.ConnectionString = ConnectionString_
            If TestDbConnection() Then
                ' Action on test success
                result = True
            Else
                ' Action on test failed
                'MsgBox("Błąd: Niepoprawa baza danych - nie można z nią pracować.")
                Toolbox.Log("Błąd: Niepoprawa baza danych - nie można z nią pracować.")
                ConnectionString_ = ConnectionString_saved
                Connection_.ConnectionString = ConnectionString_
                result = False
            End If
        End If
        Return result
    End Function

    Public Function CreateNewDataBase(ByVal path_to_data_base As String) As Boolean

        Dim result As Boolean = False
        If My.Computer.FileSystem.FileExists(path_to_data_base) Then
            'MsgBox("Błąd: Plik już istnieje") 
            Toolbox.Log("Błąd: Plik już istnieje")
            Return False
        End If
        SQLiteConnection.CreateFile(path_to_data_base)
        dataBaseName_ = path_to_data_base

        ConnectionString_ = "DataSource=" + dataBaseName_ + ";Version=3;New=False;Compress=True;"
        Connection_.ConnectionString = ConnectionString_
        Dim rows_affected As Integer = CreateDb(ConnectionString_)
        MsgBox("Info: Tworzenie bazy danych (Kod: " & rows_affected & ")")
        isSetUp_ = True
        result = True
        Return result

    End Function

    'Public Function SetUp(ByVal path_to_data_base As String) As Boolean

    '    dataBaseName_ = path_to_data_base

    '    ' 1. Check if file exists
    '    If Not My.Computer.FileSystem.FileExists(dataBaseName_) Then
    '        SQLiteConnection.CreateFile(dataBaseName_)
    '        IsDataBaseInitialized_ = False
    '        MsgBox("Baza danych została utworzona: " & dataBaseName_)
    '    End If
    '    ' 2. Define database
    '    Dim ConnectionString As String = "DataSource=" + dataBaseName_ + ";Version=3;New=False;Compress=True;"

    '    If Not IsDataBaseInitialized_ Then
    '        Dim ret_val As Integer = CreateDb(ConnectionString)
    '        MsgBox("Baza danych została zdefiniowana (Kod: " & ret_val & ")")
    '        IsDataBaseInitialized_ = True
    '    End If

    '    Connection_ = New SQLiteConnection
    '    Connection_.ConnectionString = ConnectionString
    '    Try
    '        Connection_.Open()
    '        Connection_.Close()
    '        isSetUp_ = True
    '    Catch ex As Exception
    '        ' If file has wron format 
    '        Console.WriteLine("[DbSqliteInterface] Error database file has wrong format " + ex.ToString())
    '        ' Wyglada na to, ze to nie zadziala.
    '    End Try

    '    Return IsDataBaseInitialized_
    'End Function

    Public Sub TearDown()

    End Sub

    'Function IsDbConnectionInitialzied() As Boolean
    '    Dim result As Boolean = False
    '    If Not Connection_ Is Nothing Then
    '        result = True
    '    End If
    '    Return result
    'End Function

    'Function OpenConnection() As Boolean

    '    If Connection_ Is Nothing Then
    '        If Not My.Computer.FileSystem.FileExists(dataBaseName_) Then
    '            SQLiteConnection.CreateFile(dataBaseName_)
    '        End If
    '        Connection_ = New SQLiteConnection
    '        Connection_.ConnectionString = ConnectionString_
    '        Try
    '            Connection_.Open()
    '        Catch ex As Exception
    '            ' If file has wron format 
    '            Console.WriteLine("[DbSqliteInterface] Error database file has wrong format " + ex.ToString())
    '            ' Wyglada na to, ze to nie zadziala.
    '        End Try

    '        Dim sqlQuerry As String = "SELECT *FROM WARD;"
    '        Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, Connection_)
    '        Dim sqlReader As SQLiteDataReader
    '        Try
    '            sqlReader = cmdSqlite.ExecuteReader()
    '        Catch ex As Exception
    '            Console.WriteLine("[DbSqliteInterface] Error database is not valid " + ex.ToString())
    '            Connection_.Close()
    '            CreateDb()
    '            Connection_.Open()
    '            Return False
    '        End Try
    '    End If
    '    IsDataBaseOpened_ = True
    '    Return IsDataBaseOpened_
    'End Function

    'Public Sub CloseConnection()
    '    If Not Connection_ Is Nothing Then
    '        Connection_.Close()
    '        IsDataBaseOpened_ = False
    '    End If
    'End Sub


    Public Function ExecuteQuerry(querry As String) As DataTable

        Console.WriteLine("[DbSqliteInterface] ExecuteQuerry")
        'OpenConnection()

        Dim sqlQuerry As String = querry
        Dim table As New DataTable
        Dim sqlReader As SQLiteDataReader
        If isSetUp_ Then
            If Not Connection_ Is Nothing Then
                Try
                    Connection_.Open()
                    Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, Connection_)
                    sqlReader = cmdSqlite.ExecuteReader()
                Catch ex As Exception
                    Connection_.Close()
                    Console.WriteLine("[DbSqliteInterface::ExecuteQuerry] Error occured" + ex.ToString())
                End Try
            Else
                MsgBox("DB: Błąd wewnętrzny bazy danych - połączenie nie jest zaincjalizowane")
            End If

            If Not sqlReader Is Nothing Then
                If sqlReader.HasRows Then
                    table.Load(sqlReader)
                End If
            End If
            Connection_.Close()
        Else
            MsgBox("Brak konfiguracji bazy danych.")
        End If
        Return table
    End Function

    Public Function GetNextFreeIndexForTable(ByVal tableName As String) As Integer

        Dim GetNextindexQuerry As String = "Select MAX(ID) FROM " + tableName + ";"
        Dim result = ExecuteQuerry(GetNextindexQuerry)

        'For debugging
        ' Console.WriteLine("IDS : " + result.Rows(0).ItemArray.ElementAt(0).ToString)
        Dim ID As Integer = -1
        If Not result Is Nothing And result.Rows.Count > 0 Then
            If Not IsDBNull(result.Rows(0).ItemArray.ElementAt(0)) Then
                'Do success
                Dim ID_db As Object = CInt(result.Rows(0).ItemArray.ElementAt(0))
                If Not ID_db Is Nothing Then
                    ID = CInt(ID_db)
                End If
            Else
                'Failure
                ID = 0
            End If
        Else
            ID = 0
        End If
        result.Clear()
        result.Dispose()
        Console.WriteLine("ID max : " + ID.ToString)
        Return ID + 1 ' Number is increased becasuse we want to get next _free_ ID.
    End Function

    'Public Sub PrintDbLogs()

    '    Console.WriteLine("DbSqliteInterface")
    '    Dim conn = New SQLiteConnection("Data Source=C:\Users\Sebastian\Documents\Projekty\Andrzeja-Elpis\Tests\SQLiteDbTests\ErisHistorian.sqlite;Version=3")
    '    conn.Open()

    '    Dim sqlQuerry As String = "SELECT *FROM LOGS;"
    '    Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, conn)
    '    Dim sqlReader As SQLiteDataReader
    '    Try
    '        'cmdSqlite.ExecuteNonQuery()
    '        sqlReader = cmdSqlite.ExecuteReader()
    '    Catch ex As Exception
    '        Console.WriteLine("[DbSqliteInterface] Error occured" + ex.ToString())
    '    End Try
    '    While (sqlReader.Read())
    '        Console.WriteLine("Row: " & sqlReader("id") & " " & sqlReader("timestamp") & " " & sqlReader("type") & " " & sqlReader("descr"))
    '    End While
    '    conn.Close()

    'End Sub

    'Public Sub TestDb()

    '    Console.WriteLine("DbSqliteInterface")
    '    'Dim commMakeDb As SQLiteConnection = New SQLiteConnection("C:\Users\Sebastian\Documents\Projekty\Andrzeja-Elpis\Tests\SQLiteDbTests\ErisHistorian.sqlite")
    '    'Dim commMakeDb As SQLiteConnection = New SQLiteConnection("VbErisHistorian.sqlite")
    '    'Dim conn = New SQLiteConnection("DataSource=ElpisDb.db3;Version=3;New=False;Compress=True;")

    '    OpenConnection()
    '    'conn.Open()

    '    Dim sqlQuerry As String = "SELECT *FROM WARD;"
    '    Dim cmdSqlite As SQLiteCommand = New SQLiteCommand(sqlQuerry, Connection_)
    '    Dim sqlReader As SQLiteDataReader
    '    Try
    '        'cmdSqlite.ExecuteNonQuery()
    '        sqlReader = cmdSqlite.ExecuteReader()
    '    Catch ex As Exception
    '        Console.WriteLine("[DbSqliteInterface] Error occured" + ex.ToString())
    '    End Try
    '    If (sqlReader.HasRows) Then
    '        While (sqlReader.Read())
    '            ' ID_WARD, TYPE, MAX_BED_NUMBER, MEAN_BED_OCCUP, DESCR, NAME
    '            Console.WriteLine("Row: " & sqlReader("id") & " " & sqlReader("TYPE") & " " & sqlReader("MAX_BED_NUMBER") & " " & sqlReader("MEAN_BED_OCCUP"))
    '        End While
    '    End If

    '    'conn.Close()

    'End Sub

    Public Function CreateDb(ByVal data_base_connection_string As String) As Integer
        Dim rows_affecetd As Integer = -1
        Using Query As New SQLiteCommand()
            Dim Connection As New SQLiteConnection()
            Connection.ConnectionString = data_base_connection_string
            Connection.Open()
            With Query
                .Connection = Connection
                .CommandText = "DROP TABLE  IF  EXISTS PATIENT;  
                                DROP TABLE  IF  EXISTS LOG;" &
                                Ward.FormQuerry_DropAndCreateTable() &
                                Room.FormQuerry_DropAndCreateTable() &
                                HospitalStaff.FormQuerry_DropAndCreateTable() &
                                Visit.FormQuerry_DropAndCreateTable() &
                                LabTest.FormQuerry_DropAndCreateTable() &
                                "CREATE TABLE IF NOT EXISTS PATIENT ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_PATIENT INTEGER NOT NULL,
                                 REGISTER_TYPE TEXT NOT NULL,
                                 IDENTIFICATION TEXT,
                                 DESCR TEXT,
                                 TEST_ID INTEGER 
                                );" &
                                Duty.FormQuerry_DropAndCreateTable() &
                                "CREATE TABLE IF NOT EXISTS LOG(
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT, 
                                 TIMESTAMP DATETIME NOT NULL, 
                                 TYPE INTEGER NOT NULL, 
                                 DESCR VCHAR(255)
                                );"
            End With
            rows_affecetd = Query.ExecuteNonQuery() ' Tutaj pojawi sie wyjatek, jesli plik jest niepoprawny. Przy inicjalizacji bazy..
            'Console.WriteLine("[SQL Connector:] Data base was initialized")
            Connection.Close()
        End Using
        'Console.WriteLine("[SQL Connector:] Data base was initialized")
        Return rows_affecetd

    End Function


    Public Function CreateDb_Oryg(ByVal data_base_connection_string As String) As Integer
        Dim rows_affecetd As Integer = -1
        Using Query As New SQLiteCommand()
            Dim Connection As New SQLiteConnection()
            Connection.ConnectionString = data_base_connection_string
            Connection.Open()
            With Query
                .Connection = Connection
                .CommandText = "DROP TABLE  IF  EXISTS WARD;
                                DROP TABLE  IF  EXISTS ROOM;
                                DROP TABLE  IF  EXISTS STAFF;
                                DROP TABLE  IF  EXISTS PATIENT;
                                DROP TABLE  IF  EXISTS VISIT;
                                DROP TABLE  IF  EXISTS TEST;
                                DROP TABLE  IF  EXISTS LOG;

                                -- CREATE TABLES 
                                CREATE TABLE IF NOT EXISTS WARD( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_WARD INTEGER NOT NULL,
                                 TYPE TEXT NOT NULL,
                                 MAX_BED_NUMBER INTEGER NOT NULL,
                                 MEAN_BED_OCCUP INTEGER,
                                 DESCR TEXT,
                                 NAME TEXT 
                                );
  
                                CREATE TABLE IF NOT EXISTS ROOM ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_ROOM INTEGER NOT NULL,
                                 TYPE TEXT NOT NULL,
                                 ID_WARD INTEGER,
                                 NUMBER INTEGER,
                                 MAX_BED_NUMBER INTEGER,
                                 MEAN_BED_OCCUP INTEGER 
                                );
  
                                CREATE TABLE IF NOT EXISTS STAFF ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_STAFF INTEGER NOT NULL,
                                 TYPE TEXT NOT NULL,
                                 WARD_ID INTEGER,
                                 NAME TEXT 
                                );
  
                                CREATE TABLE IF NOT EXISTS PATIENT ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_PATIENT INTEGER NOT NULL,
                                 REGISTER_TYPE TEXT NOT NULL,
                                 IDENTIFICATION TEXT,
                                 DESCR TEXT,
                                 TEST_ID INTEGER 
                                );
  
                                CREATE TABLE IF NOT EXISTS VISIT ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 ID_HOSP INTEGER NOT NULL,
                                 PATIENT_ID INTEGER NOT NULL,
                                 IN_DATE DATE NOT NULL,
                                 OUT_DATE DATE,
                                 NAME TEXT 
                                );
  
                                CREATE TABLE IF NOT EXISTS TEST ( 
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 PATIENT_ID INTEGER NOT NULL,
                                 P1_DATE DATE,
                                 P1_RESULT_BIN INTEGER,
                                 P1_RESULT_NOMINAL INTEGER,
                                 P1_KIND TEXT,
                                 P2_DATE DATE,
                                 P2_RESULT_BIN INTEGER,
                                 P2_RESULT_NOMINAL INTEGER,
                                 P2_KIND TEXT 
                                );

                                CREATE TABLE IF NOT EXISTS DUTY (  
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT,
                                 DATE DATE NOT NULL,
                                 STAFF_ID INTEGER NOT NULL,
                                 HAS_DUTY BOOLEAN NOT NULL 
                                );  

                                ------------
                                CREATE TABLE IF NOT EXISTS LOG(
                                 ID INTEGER PRIMARY KEY AUTOINCREMENT, 
                                 TIMESTAMP DATETIME NOT NULL, 
                                 TYPE INTEGER NOT NULL, 
                                 DESCR VCHAR(255)
                                );"
            End With
            rows_affecetd = Query.ExecuteNonQuery() ' Tutaj pojawi sie wyjatek, jesli plik jest niepoprawny. Przy inicjalizacji bazy..
            'Console.WriteLine("[SQL Connector:] Data base was initialized")
            Connection.Close()
        End Using
        'Console.WriteLine("[SQL Connector:] Data base was initialized")
        Return rows_affecetd

    End Function

End Class
