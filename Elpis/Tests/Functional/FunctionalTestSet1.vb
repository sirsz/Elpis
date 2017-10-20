Module FunctionalTestSet1

    Class FunctionalTestSet

        Sub WardClear_Test()
            Dim ward As New Ward
            ward.Clear()
        End Sub

        Sub WardFormInsertQuerry_Test()
            Dim ward As New Ward
            Dim querry As String = ward.FormQuerry_Insert()
            WriteLine(querry)
        End Sub

        Sub WardFormUpdateQuerry_Test()
            Dim ward As New Ward
            Dim querry As String = ward.FormQuerry_Update()
            WriteLine(querry)
        End Sub

        Sub WardFormDeleteQuerry_Test()
            Dim ward As New Ward
            Dim querry As String = ward.FormQuerry_Delete()
            WriteLine(querry)
        End Sub

        'Sub test1()

        '    'Dim room1 As Room = New Room(1, 1)
        '    'Dim room2 As Room = New Room(2, 2)

        '    'Dim ward1 As Ward = New Ward(1, "SOR")
        '    'ward1.AddRoom(room1)
        '    'ward1.AddRoom(room2)

        '    Dim hospital As Hospital = New Hospital(1, "Kliniczny")
        '    hospital.AddWard(ward1)

        '    hospital.ListWard()


        'End Sub
    End Class

End Module
