package cz.cvut.fit.tjv.realestates.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.time.LocalDate;
import java.util.List;

@Getter
@AllArgsConstructor
public class CustomerDTO {
    private final String email;
    private final String firstName;
    private final String lastName;
    private final LocalDate birthDate;
    private final List<Long> idSubleases;
}
