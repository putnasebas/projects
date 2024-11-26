package cz.cvut.fit.tjv.realestates.controller.dto;

import cz.cvut.fit.tjv.realestates.domain.Property;
import lombok.AllArgsConstructor;
import lombok.Getter;

import java.time.LocalDate;
import java.util.List;

@Getter
@AllArgsConstructor
public class CustomerDTO {
    final private String email;
    final private String firstName;
    final private String lastName;
    final private LocalDate birthDate;
    private final List<Long> idSubleases;
}
