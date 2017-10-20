

Imports System.Collections.Generic

Public Enum HospitalStaffType
    Undefined
    Lekarska
    Pielęgniarska
    Other
End Enum


Public Class HospitalStaff

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_fullname_ As String
    Public f_id_ As Integer
    Public f_type_ As HospitalStaffType
    Public f_ward_id_ As Integer ' forgein key of <-> Ward.f_id_ 

    Private table_name_ As String = "STAFF"

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

    Function Get_f_fullname() As String
        Return f_fullname_
    End Function

    Function Get_f_Type() As String
        Return f_type_.ToString
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Private cQuote As String = Chr(34)


    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_fullname As String,
        ByVal f_type As String,
        Optional ByVal f_ward_id As String = ""
    ) As Boolean
        Dim f_id_local As Integer
        Dim f_fullname_local As String = ""
        Dim f_type_local As HospitalStaffType
        Dim f_ward_id_local As Integer = -1

        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If
            f_fullname_local = f_fullname

            If f_type = "Lekarska" Then
                f_type_local = HospitalStaffType.Lekarska
            ElseIf f_type = "Pielęgniarska" Then
                f_type_local = HospitalStaffType.Pielęgniarska
            ElseIf f_type = "Other" Then
                f_type_local = HospitalStaffType.Other
            Else
                f_type_ = HospitalStaffType.Undefined
            End If

            If f_ward_id = String.Empty Then
                f_ward_id_local = -1
            Else
                f_ward_id_local = CInt(f_ward_id)
            End If

        Catch ex As Exception
            validation_passed = False
            MsgBox("Hospital staff validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
        End Try
        If validation_passed Then
            Update(f_id_local, f_fullname_local, f_type_local, f_ward_id_local)
        End If
        Return validation_passed
    End Function


    Sub Update(
        ByVal f_id As Integer,
        ByVal f_fullname As String,
        ByVal f_type As HospitalStaffType,
        Optional ByVal f_ward_id As Integer = -1
    )
        f_id_ = f_id
        f_fullname_ = f_fullname
        f_type_ = f_type
        f_ward_id_ = f_ward_id

    End Sub

    Sub Update(
        ByVal d_id As Integer,
        ByVal f_id As Integer,
        ByVal f_fullname As String,
        ByVal f_type As HospitalStaffType,
        Optional ByVal f_ward_id As Integer = -1
    )
        'id_ = id
        d_id_ = d_id
        f_id_ = f_id
        f_fullname_ = f_fullname
        f_type_ = f_type
        f_ward_id_ = f_ward_id

    End Sub

    Sub Clear() ' Reset class fields
        'id_ = 0
        d_id_ = -1
        f_fullname_ = String.Empty
        f_id_ = -1
        f_type_ = HospitalStaffType.Undefined
        f_ward_id_ = -1
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String
        Dim f_id_local As String
        Dim f_fullname_local As String
        Dim f_type_local As String
        Dim f_ward_id_local As String

        Dim parts As String() = Toolbox.SplitString(s)
        d_id_local = parts(0)
        f_id_local = parts(1)
        f_fullname_local = parts(4)
        f_type_local = parts(2)
        f_ward_id_local = parts(3)

        ValidateAndUpdate(f_id_local, f_fullname_local, f_type_local, f_ward_id_local)
        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE STAFF QUERRY ORDER: |                 0                 |      1           |      2        |    3    |  4   |
        ' THE STAFF QUERRY ORDER: |                 ID                |     ID_STAFF     |     TYPE      | WARD_ID | NAME | 
        ' THE STAFF QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | TEXT Not NULL | INTEGER | TEXT | 

        Dim querry As String = String.Empty
        querry = "SELECT ID, ID_STAFF, TYPE, WARD_ID, NAME FROM STAFF"
        If f_id_ > -1 Then
            querry += " WHERE ID_STAFF = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty

        querry = "INSERT INTO STAFF (ID_STAFF, TYPE, WARD_ID, NAME) VALUES (" +
            f_id_.ToString & "," &
            cQuote & f_type_.ToString & cQuote & "," &
            f_ward_id_.ToString & "," &
            cQuote & f_fullname_ & cQuote &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & HospitalStaffType.ToString() & quote & ", " &

        querry = "UPDATE STAFF SET " &
         " ID_STAFF = " & f_id_.ToString() & ", " &
         " TYPE = " & cQuote & f_type_.ToString() & cQuote & ", " &
         " WARD_ID = " & f_ward_id_.ToString() & ", " &
         " NAME = " & cQuote & f_fullname_ & cQuote & " " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM STAFF " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE If EXISTS STAFF; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE IF NOT EXISTS STAFF ( 
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,
                    ID_STAFF INTEGER NOT NULL,
                    TYPE TEXT NOT NULL,
                    WARD_ID INTEGER,
                    NAME TEXT 
                 );"
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class

