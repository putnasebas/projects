package cz.cvut.fit.tjv.realestates.client.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDate;
import java.util.List;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class CustomerDTO {
    private String email;
    private String firstName;
    private String lastName;
    private LocalDate birthDate;
    private List<Long> idSubleases = List.of();

}
