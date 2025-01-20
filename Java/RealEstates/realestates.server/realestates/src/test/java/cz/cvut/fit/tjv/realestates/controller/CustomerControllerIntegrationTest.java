package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.CustomerService;
import cz.cvut.fit.tjv.realestates.controller.converter.CustomerDTOConverter;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import jakarta.persistence.EntityNotFoundException;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;

import java.time.LocalDate;
import java.util.List;

import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
@AutoConfigureMockMvc
public class CustomerControllerIntegrationTest {
    // Simulates HTTP requests
    @Autowired
    private MockMvc mockMvc;
    // Avoids running tests on production database
    @MockBean
    private CustomerService customerService;
    private CustomerController controller;
    private CustomerDTOConverter customerDTOConverter;
    @Autowired
    private CustomerController customerController;

    @Test
    public void testCustomerGetAll() throws Exception {
        when(customerService.getAllEntities()).thenReturn(
                List.of(
                        new Customer("John.Doe@mail.com", "John", "Doe",
                                LocalDate.of(1989, 6, 30), List.of()),
                        new Customer("Jan.Novak@mail.com", "Jan", "Novak",
                                LocalDate.of(1999, 4, 11), List.of()),
                        new Customer("Jana.Novakova@mail.com", "Jana", "Novakova",
                                LocalDate.of(2001, 4, 5), List.of())
                )
        );
        mockMvc.perform(get("/rest/api/customer")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[0].email").value("John.Doe@mail.com")
        );
        mockMvc.perform(get("/rest/api/customer")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[1].email").value("Jan.Novak@mail.com")
        );
        mockMvc.perform(get("/rest/api/customer")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[2].email").value("Jana.Novakova@mail.com")
        );
    }

    @Test
    public void testCustomerGet() throws Exception {
        when(customerService.getEntityById("Java.Novak@mail.com")).thenReturn(
                new Customer("Java.Novak@mail.com", "Java", "Novak",
                        LocalDate.of(2006, 7, 15), List.of())
        );
        mockMvc.perform(get("/rest/api/customer/Java.Novak@mail.com")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$.email").value("Java.Novak@mail.com")
        );
    }

    @Test
    public void testUnknownCustomerGet() throws Exception {
        when(customerService.getEntityById("UnknownCustomer@mail.com")).thenThrow(EntityNotFoundException.class);
        mockMvc.perform(get("/rest/api/customer/UnknownCustomer@mail.com")).andExpect(status().isNotFound());
    }

}
