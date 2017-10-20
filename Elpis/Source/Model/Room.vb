

Imports System.Collections.Generic

Public Enum RoomType
    Undefined
    Type1
    Type2
    Type3
End Enum


Public Class Room

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_number_ As String
    Public f_id_ As Integer
    Public f_type_ As RoomType
    Public f_bed_number_max_ As UInteger
    Public f_bed_number_mean_ As UInteger
    Public f_ward_id_ As Integer ' forgein key of <-> Ward.f_id_
    'Public address_ As String
    'Public description_ As String

    Private table_name_ As String = "ROOM"

    Function IsInitialized() As Boolean
        Return d_id_ > -1
    End Function


    Function Get_d_Id() As String
        Return d_id_.ToString
    End Function

    Function Get_f_Id_Ward() As String
        Return f_ward_id_.ToString
    End Function

    Function Get_f_Id() As String
        Return f_id_.ToString
    End Function

    Function Get_f_number() As String
        Return f_number_
    End Function

    Function Get_f_Type() As String
        Return f_type_.ToString
    End Function

    Function Get_f_bed_number_max() As String
        Return f_bed_number_max_.ToString
    End Function

    Function Get_f_bed_number_mean() As String
        Return f_bed_number_mean_.ToString
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Private cQuote As String = Chr(34)


    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_number As String,
        ByVal f_type As String,
        ByVal f_bed_number_max As String,
        ByVal f_bed_number_mean As String,
        Optional ByVal f_ward_id As String = ""
    ) As Boolean
        Dim f_id_local As Integer
        Dim f_number_local As String = ""
        Dim f_type_local As RoomType
        Dim f_bed_number_max_local As UInteger
        Dim f_bed_number_mean_local As UInteger
        Dim f_ward_id_local As Integer = -1

        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If
            f_number_local = f_number
            If f_type = "Type1" Then
                f_type_local = RoomType.Type1
            ElseIf f_type = "Type2" Then
                f_type_local = RoomType.Type2
            ElseIf f_type = "Type3" Then
                f_type_local = RoomType.Type3
            Else
                f_type_local = RoomType.Undefined
            End If
            If f_bed_number_max = String.Empty Then
                f_bed_number_max_local = 0
            Else
                f_bed_number_max_local = CInt(f_bed_number_max)
            End If
            If f_bed_number_mean = String.Empty Then
                f_bed_number_mean_local = 0
            Else
                f_bed_number_mean_local = CInt(f_bed_number_mean)
            End If
            If f_ward_id = String.Empty Then
                f_ward_id_local = -1
            Else
                f_ward_id_local = CInt(f_ward_id)
            End If

        Catch ex As Exception
            validation_passed = False
            MsgBox("Room validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
        End Try
        If validation_passed Then
            Update(f_id_local, f_number_local, f_type_local, f_bed_number_max_local, f_bed_number_mean_local, f_ward_id_local)
        End If
        Return validation_passed
    End Function


    Sub Update(
        ByVal f_id As Integer,
        ByVal f_number As String,
        ByVal f_type As RoomType,
        ByVal f_bed_number_max As UInteger,
        ByVal f_bed_number_mean As UInteger,
        Optional ByVal f_ward_id As Integer = -1
    )
        f_id_ = f_id
        f_number_ = f_number
        f_type_ = f_type
        f_bed_number_max_ = f_bed_number_max
        f_bed_number_mean_ = f_bed_number_mean
        f_ward_id_ = f_ward_id

    End Sub

    Sub Update(
        ByVal d_id As Integer,
        ByVal f_id As Integer,
        ByVal f_number As String,
        ByVal f_type As RoomType,
        ByVal f_bed_number_max As UInteger,
        ByVal f_bed_number_mean As UInteger,
        Optional ByVal f_ward_id As Integer = -1
    )
        'id_ = id
        d_id_ = d_id
        f_id_ = f_id
        f_number_ = f_number
        f_type_ = f_type
        f_bed_number_max_ = f_bed_number_max
        f_bed_number_mean_ = f_bed_number_mean
        f_ward_id_ = f_ward_id

    End Sub

    Sub Clear() ' Reset class fields
        'id_ = 0
        d_id_ = -1
        f_number_ = String.Empty
        f_id_ = -1
        f_type_ = RoomType.Undefined
        f_bed_number_max_ = 0
        f_bed_number_mean_ = 0
        f_ward_id_ = -1
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String
        Dim f_id_local As String
        Dim f_number_local As String
        Dim f_type_local As String
        Dim f_bed_number_max_local As String
        Dim f_bed_number_mean_local As String
        Dim f_ward_id_local As String

        Dim parts As String() = Toolbox.SplitString(s)
        d_id_local = parts(0)
        f_id_local = parts(1)
        f_number_local = parts(4)
        f_bed_number_max_local = parts(5)
        f_bed_number_mean_local = parts(6)
        f_type_local = parts(2)
        f_ward_id_local = parts(3)

        ValidateAndUpdate(f_id_local, f_number_local, f_type_local, f_bed_number_max_local, f_bed_number_mean_local, f_ward_id_local)
        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE ROOM QUERRY ORDER: |                 0                 |     1            |      2        |    3    |    4    |      5         |     6          |      
        ' THE ROOM QUERRY ORDER: |                 ID                |     ID_ROOM      |      TYPE     | ID_WARD | NUMBER  | MAX_BED_NUMBER | MEAN_BED_OCCUP |
        ' THE ROOM QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | TEXT Not NULL | INTEGER | INTEGER | INTEGER        | INTEGER        |

        Dim querry As String = String.Empty
        querry = "SELECT  ID, ID_ROOM, TYPE, ID_WARD, NUMBER, MAX_BED_NUMBER, MEAN_BED_OCCUP FROM ROOM"
        If f_id_ > -1 Then
            querry += " WHERE ID_ROOM = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty
        'TODO: 
        'querry = "INSERT INTO WARD(ID,ID_ROOM,TYPE,MAX_BED_NUMBER,MEAN_BED_OCCUP,NAME) VALUES (" +
        'When inserting d_id or (ID) shall be autoincremented ID.ToString & "," &

        querry = "INSERT INTO ROOM (ID_ROOM, TYPE, ID_WARD, NUMBER, MAX_BED_NUMBER, MEAN_BED_OCCUP) VALUES (" +
            f_id_.ToString & "," &
            cQuote & f_type_ & cQuote & "," &
            f_ward_id_.ToString & "," &
            cQuote & f_number_ & cQuote & "," &
            f_bed_number_max_.ToString & "," &
            f_bed_number_mean_.ToString & " " &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & RoomType.ToString() & quote & ", " &

        querry = "UPDATE ROOM SET " &
         " ID_ROOM = " & f_id_.ToString() & ", " &
         " TYPE = " & cQuote & f_type_.ToString() & cQuote & ", " &
         " ID_WARD = " & f_ward_id_.ToString() & ", " &
         " NUMBER = " & cQuote & f_number_ & cQuote & " " &
         " MAX_BED_NUMBER = " & f_bed_number_max_.ToString() & ", " &
         " MEAN_BED_OCCUP = " & f_bed_number_mean_.ToString & ", " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM ROOM " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE If EXISTS ROOM; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE IF NOT EXISTS ROOM ( 
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,
                    ID_ROOM INTEGER NOT NULL,
                    TYPE TEXT NOT NULL,
                    ID_WARD INTEGER,
                    NUMBER TEXT,
                    MAX_BED_NUMBER INTEGER,
                    MEAN_BED_OCCUP INTEGER 
                ); "
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class

'Public Class Room

'    Public id_ As UInteger
'    Public number_ As UInteger
'    Public name_ As String
'    Public description_ As String

'    Sub New(ByVal id As UInteger, ByVal number As UInteger)
'        id_ = id
'        number_ = number
'    End Sub
'    Sub New(ByVal id As UInteger, ByVal number As UInteger, ByVal name As String)
'        id_ = id
'        number_ = number
'        name_ = name
'    End Sub

'End Class
