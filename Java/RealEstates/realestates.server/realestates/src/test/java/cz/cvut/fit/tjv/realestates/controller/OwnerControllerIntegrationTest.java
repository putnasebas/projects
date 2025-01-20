package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import jakarta.persistence.EntityNotFoundException;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;

import java.util.List;

import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
@AutoConfigureMockMvc
public class OwnerControllerIntegrationTest {
    @Autowired
    private MockMvc mockMvc;
    @MockBean
    private OwnerService ownerService;

    @Test
    public void testOwnerGetAll() throws Exception {
        when(ownerService.getAllEntities()).thenReturn(
                List.of(
                        new Owner("First.Owner@mail.com", null, List.of()),
                        new Owner("Second.Owner@mail.com", null, List.of()),
                        new Owner("Third.Owner@mail.com", null, List.of())
                )
        );
        mockMvc.perform(get("/rest/api/owner")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[0].email").value("First.Owner@mail.com")
        );
        mockMvc.perform(get("/rest/api/owner")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[1].email").value("Second.Owner@mail.com")
        );
        mockMvc.perform(get("/rest/api/owner")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[2].email").value("Third.Owner@mail.com")
        );
    }

    @Test
    public void testOwnerGet() throws Exception {
        when(ownerService.getEntityById("TestOwner@mail.com")).thenReturn(
                new Owner("TestOwner@mail.com", null, List.of())
        );
        mockMvc.perform(get("/rest/api/owner/TestOwner@mail.com")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$.email").value("TestOwner@mail.com")
        );
    }

    @Test
    public void testUnknownOwnerGet() throws Exception {
        when(ownerService.getEntityById("UnknownOwner@mail.com")).thenThrow(EntityNotFoundException.class);
        mockMvc.perform(get("/rest/api/owner/UnknownOwner@mail.com")).andExpect(status().isNotFound());
    }
}
