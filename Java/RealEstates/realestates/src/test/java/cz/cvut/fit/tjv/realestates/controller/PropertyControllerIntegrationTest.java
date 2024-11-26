package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.PropertyService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;


@SpringBootTest
@AutoConfigureMockMvc
public class PropertyControllerIntegrationTest {
    @Autowired
    private MockMvc mockMvc;
    @MockBean
    private PropertyService propertyService;

    @Test
    public void testPropertyGetAll() {

    }

    @Test
    public void testPropertyGet() {

    }

    @Test
    public void testPropertyCreate() {

    }

    @Test
    public void testPropertyUpdate() {

    }

    @Test
    public void testPropertyDelete() {

    }

}
