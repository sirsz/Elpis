
Imports System.Collections.Generic

'Public Enum WardType
'    Undefined
'    Type1
'    Type2
'    Type3
'End Enum


Public Class Ward

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_name_ As String
    Public f_id_ As Integer
    Public f_type_ As String
    Public f_bed_number_max_ As UInteger
    Public f_bed_number_mean_ As UInteger
    'Public address_ As String
    'Public description_ As String

    Private table_name_ As String = "WARD"

    Function IsInitialized() As Boolean
        Return d_id_ > -1
    End Function


    Function Get_d_Id() As String
        Return d_id_.ToString
    End Function

    Function Get_f_Id() As String
        Return f_id_.ToString
    End Function

    Function Get_f_Name() As String
        Return f_name_
    End Function

    Function Get_f_Type() As String
        Return f_type_.ToString
    End Function

    Function Get_f_bed_number_max_() As String
        Return f_bed_number_max_.ToString
    End Function

    Function Get_f_bed_number_mean_() As String
        Return f_bed_number_mean_.ToString
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Private cQuote As String = Chr(34)


    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_name As String,
        ByVal f_type As String,
        ByVal f_bed_number_max As String,
        ByVal f_bed_number_mean As String
    ) As Boolean
        Dim f_id_local As Integer = -1
        Dim f_name_local As String = String.Empty
        Dim f_type_local As String = String.Empty
        Dim f_bed_number_max_local As UInteger
        Dim f_bed_number_mean_local As UInteger

        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If

            f_name_local = f_name
            f_type_local = f_type

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
        Catch ex As Exception
            validation_passed = False
            MsgBox("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
        End Try
        If validation_passed Then
            Update(f_id_local, f_name_local, f_type_local, f_bed_number_max_local, f_bed_number_mean_local)
        End If
        Return validation_passed
    End Function


    'Function IsSame(
    '    ByVal f_id As String,
    '    ByVal f_name As String,
    '    ByVal f_type As String,
    '    ByVal f_bed_number_max As String,
    '    ByVal f_bed_number_mean As String
    ') As Boolean
    '    Dim compare_result As Boolean = False
    '    ' TODO: CAST TO PROPER VAULES
    '    If f_id_ = f_id And
    '       f_name_ = f_name And
    '       f_type_ = f_type And
    '       f_bed_number_max_ = f_bed_number_max And
    '       f_bed_number_mean_ = f_bed_number_mean Then
    '        compare_result = True
    '    End If
    '    Return compare_result
    'End Function


    Sub Update(
        ByVal f_id As Integer,
        ByVal f_name As String,
        ByVal f_type As String,
        ByVal f_bed_number_max As UInteger,
        ByVal f_bed_number_mean As UInteger
    )
        f_id_ = f_id
        f_name_ = f_name
        f_type_ = f_type
        f_bed_number_max_ = f_bed_number_max
        f_bed_number_mean_ = f_bed_number_mean

    End Sub

    Sub Update(
        ByVal d_id As Integer,
        ByVal f_id As Integer,
        ByVal f_name As String,
        ByVal f_type As String,
        ByVal f_bed_number_max As UInteger,
        ByVal f_bed_number_mean As UInteger
    )
        'id_ = id
        d_id_ = d_id
        f_id_ = f_id
        f_name_ = f_name
        f_type_ = f_type
        f_bed_number_max_ = f_bed_number_max
        f_bed_number_mean_ = f_bed_number_mean

    End Sub

    Sub Clear() ' Reset class fields
        'id_ = 0
        d_id_ = -1
        f_name_ = String.Empty
        f_id_ = -1
        f_type_ = String.Empty
        f_bed_number_max_ = 0
        f_bed_number_mean_ = 0
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String
        Dim f_id_local As String
        Dim f_name_local As String
        Dim f_type_local As String
        Dim f_bed_number_max_local As String
        Dim f_bed_number_mean_local As String

        Dim parts As String() = Toolbox.SplitString(s)
        d_id_local = parts(0)
        f_id_local = parts(1)
        f_name_local = parts(6)
        f_bed_number_max_local = parts(3)
        f_bed_number_mean_local = parts(4)
        f_type_local = parts(2)

        ValidateAndUpdate(f_id_local, f_name_local, f_type_local, f_bed_number_max_local, f_bed_number_mean_local)
        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE WARD QUERRY ORDER: | 0  | 1       |   2  |   3            |   4            |     5 |  6   | 
        ' THE WARD QUERRY ORDER: | ID | ID_WARD | TYPE | MAX_BED_NUMBER | MEAN_BED_OCCUP | DESCR | NAME | 

        Dim querry As String = String.Empty
        querry = "SELECT ID, ID_WARD, TYPE, MAX_BED_NUMBER, MEAN_BED_OCCUP, DESCR, NAME FROM WARD"
        If f_id_ > -1 Then
            querry += " WHERE ID_WARD = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty
        'TODO: 
        'querry = "INSERT INTO WARD(ID,ID_WARD,TYPE,MAX_BED_NUMBER,MEAN_BED_OCCUP,NAME) VALUES (" +
        'When inserting d_id or (ID) shall be autoincremented ID.ToString & "," &

        querry = "INSERT INTO WARD (ID_WARD, TYPE, MAX_BED_NUMBER, MEAN_BED_OCCUP, NAME) VALUES (" +
            f_id_.ToString & "," &
            cQuote & f_type_ & cQuote & "," &
            f_bed_number_max_.ToString & "," &
            f_bed_number_mean_.ToString & "," &
            cQuote & f_name_ & cQuote &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & WardType.ToString() & quote & ", " &

        querry = "UPDATE WARD SET " &
         " ID_WARD = " & f_id_.ToString() & ", " &
         " TYPE = " & cQuote & f_type_.ToString() & cQuote & ", " &
         " MAX_BED_NUMBER = " & f_bed_number_max_.ToString() & ", " &
         " MEAN_BED_OCCUP = " & f_bed_number_mean_.ToString & ", " &
         " NAME = " & cQuote & f_name_ & cQuote & " " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM WARD " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE If EXISTS WARD; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE If Not EXISTS WARD( " &
                    "ID Integer PRIMARY KEY AUTOINCREMENT," &
                    "ID_WARD Integer Not NULL," &
                    "Type TEXT Not NULL, " &
                    "MAX_BED_NUMBER Integer Not NULL," &
                    "MEAN_BED_OCCUP Integer, " &
                    "DESCR TEXT," &
                    "NAME TEXT " &
                 "); "
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class


'Public Class Ward

'    Public id_ As UInteger
'    Public name_ As String
'    Public location_ As String
'    Public description_ As String

'    Private roomList As New List(Of Room)()

'    Sub New(ByVal id As UInteger, ByVal name As String)
'        id_ = id
'        name_ = name
'    End Sub
'    Public Sub AddRoom(ByRef room)
'        roomList.Add(room)
'    End Sub

'    Public Sub ListRooms()
'        For Each room In roomList
'            Console.WriteLine(room)
'        Next
'    End Sub

'End Class
