package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.domain.Property;
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
public class PropertyControllerIntegrationTest {
    @Autowired
    private MockMvc mockMvc;
    @MockBean
    private PropertyService propertyService;

    @Test
    public void testPropertyGetAll() throws Exception {
        when(propertyService.getAllEntities()).thenReturn(
                List.of(
                        new Property(0L, 30, 40000L,
                                "Praha", "Pod Kaštany", 20, null, List.of()),
                        new Property(1L, 40, 20000L,
                                "Ústí nad Labem", "Masarykova", 148, null, List.of()),
                        new Property(2L, 25, 19500L,
                                "Litoměřice", "Družstevní", 2, null, List.of())
                )
        );
        mockMvc.perform(get("/rest/api/property")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$[0].city").value("Praha")
        );

    }

    @Test
    public void testPropertyGet() throws Exception {
        when(propertyService.getEntityById(0L)).thenReturn(new Property(0L, 30, 40000L,
                "Praha", "Pod Kaštany", 20, null, List.of()));
        mockMvc.perform(get("/rest/api/property/0")).andExpect(status().isOk()).andExpect(
                MockMvcResultMatchers.jsonPath("$.city").value("Praha")
        );
    }

    @Test
    public void testGetUnknownProperty() throws Exception {
        when(propertyService.getEntityById(0L)).thenThrow(new EntityNotFoundException());
        mockMvc.perform(get("/rest/api/property/0")).andExpect(status().isNotFound());
    }

}
